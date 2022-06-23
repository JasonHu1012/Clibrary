#include "collision.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define WINDOW_TO_DISPLAY_RATIO 0.6
#define MAX_FPS 120

struct room {
    int dim;
    int ball_num;
    double block_size;
    int *block_num;
    double *range_min;
    double *range_max;
    list *ball_list;
    ndarray *block;
};

struct ball {
    double radius;
    double mass;
    vector *position;
    vector *velocity;
    int *block_index;
    vector *new_velocity; // update velocity after calculating all balls'
    int id;
};

static void init_block_helper(ndarray *block, int *size, int dim, int cur, int *index) {
    if (cur == dim) {
        list *l = lst_init(sizeof(list_node *));
        nda_set(block, index, &l);
    }
    else {
        for (int i = 0; i < size[cur]; i++) {
            index[cur] = i;
            init_block_helper(block, size, dim, cur + 1, index);
        }
    }
}

static void init_block(ndarray *block, int *size, int dim) {
    int *index = (int *)malloc(sizeof(int) * dim);
    init_block_helper(block, size, dim, 0, index);
    free(index);
}

room *cls_init_room(int dim, double *range_min, double *range_max, double max_ball_radius) {
    assert(dim > 0);
    assert(max_ball_radius > 0);
    room *output = (room *)malloc(sizeof(room));
    output->dim = dim;
    output->ball_num = 0;
    output->block_size = max_ball_radius * 2;
    output->block_num = (int *)malloc(sizeof(int) * dim);
    output->range_min = (double *)malloc(sizeof(double) * dim);
    output->range_max = (double *)malloc(sizeof(double) * dim);
    for (int i = 0; i < dim; i++) {
        assert(range_max[i] > range_min[i]);
        output->block_num[i] = (int)ceil((range_max[i] - range_min[i]) / output->block_size);
        output->range_min[i] = range_min[i];
        output->range_max[i] = range_max[i];
    }
    output->ball_list = lst_init(sizeof(list_node *));
    output->block = nda_init(output->block_num, dim, sizeof(list *));
    init_block(output->block, output->block_num, dim);
    return output;
}

ball *cls_init_ball(double radius, double mass, vector *position, vector *velocity) {
    assert(radius > 0);
    assert(mass > 0);
    assert(vec_dim(position) == vec_dim(velocity));
    ball *output = (ball *)malloc(sizeof(ball));
    output->radius = radius;
    output->mass = mass;
    output->position = vec_copy(position);
    output->velocity = vec_copy(velocity);
    output->block_index = (int *)malloc(sizeof(int) * vec_dim(position));
    return output;
}

static bool get_block_index(room *r, ball *b) {
    int dim = r->dim;
    bool change = false;
    for (int i = 0; i < dim; i++) {
        int old_index = b->block_index[i];
        b->block_index[i] = (int)floor((*vec_entry(b->position, i + 1) - r->range_min[i]) / r->block_size);
        assert(b->block_index[i] >= 0 && b->block_index[i] < r->block_num[i]);
        if (old_index != b->block_index[i]) {
            change = true;
        }
    }
    return change;
}

void cls_add_ball(room *r, ball *b) {
    assert(r->dim == vec_dim(b->position));
    ball *clone_ball = cls_init_ball(b->radius, b->mass, b->position, b->velocity);
    clone_ball->id = ++r->ball_num;
    list_node *n = lst_init_node(&clone_ball, sizeof(ball *));
    lst_add_tail(r->ball_list, &n, NULL);
    get_block_index(r, b);
    list *l;
    nda_get(r->block, b->block_index, &l);
    lst_add_tail(l, NULL, n);
}

static void print_ball(ball *b) {
    printf("---ball %d---\n", b->id);
    printf("mass: %.2e\n", b->mass);
    printf("position: ");
    vec_print(b->position);
    printf("velocity: ");
    vec_print(b->velocity);
}

void cls_print(room *r) {
    list_iter *it = lst_init_iter(r->ball_list, false);
    list_node *n;
    while (lst_iter_next(it, &n)) {
        ball *b;
        lst_get_node(n, &b);
        print_ball(b);
    }
    lst_kill_iter(it);
}

double cls_kinetic(room *r) {
    list_iter *it = lst_init_iter(r->ball_list, false);
    list_node *n;
    double output = 0;
    while (lst_iter_next(it, &n)) {
        ball *b;
        lst_get_node(n, &b);
        output += b->mass * vec_dot(b->velocity, b->velocity) / 2;
    }
    lst_kill_iter(it);
    return output;
}

static void check_border(room *r, ball *b, vector **new_velocity) {
    for (int i = 0; i < r->dim; i++) {
        vector *normal = vec_zero(r->dim);
        if (*vec_entry(b->position, i + 1) - b->radius < r->range_min[i]) {
            *vec_entry(normal, i + 1) = 1;
        }
        else if (*vec_entry(b->position, i + 1) + b->radius > r->range_max[i]) {
            *vec_entry(normal, i + 1) = -1;
        }
        if (*vec_entry(normal, i + 1)) {
            if (vec_dot(b->velocity, normal) < 0) { // the ball is moving toward the border
                vector *proj = vec_project(b->velocity, normal);
                vector *delta = vec_mul(proj, -2);
                vec_kill(proj);
                vector *old = *new_velocity;
                *new_velocity = vec_add(old, delta);
                vec_kill(old);
                vec_kill(delta);
            }
        }
        vec_kill(normal);
    }
}

static vector *next_position(ball *b, int interval) {
    vector *delta = vec_mul(b->velocity, (double)interval / 1000000);
    vector *output = vec_add(b->position, delta);
    vec_kill(delta);
    return output;
}

static void check_neighbor_helper(room *r, ball *b, vector **new_velocity, int interval, int cur, int *index) {
    if (cur == r->dim) {
        list *l;
        nda_get(r->block, index, &l);
        list_iter *it = lst_init_iter(l, false);
        ball *neighbor;
        while (lst_iter_next(it, &neighbor)) {
            if (neighbor == b) {
                continue;
            }
            vector *d_vec = vec_minus(b->position, neighbor->position);
            if (vec_len(d_vec) < b->radius + neighbor->radius) {
                vector *b_new_position = next_position(b, interval);
                vector *n_new_position = next_position(neighbor, interval);
                vector *new_d_vec = vec_minus(b_new_position, n_new_position);
                vec_kill(b_new_position);
                vec_kill(n_new_position);
                double old_d = vec_len(d_vec);
                double new_d = vec_len(new_d_vec);
                vec_kill(new_d_vec);
                if (new_d < old_d) { // the two balls are colliding
                    vector *normal = vec_unit(d_vec);
                    vector *temp = vec_minus(neighbor->velocity, b->velocity);
                    double n = 2 * b->mass * neighbor->mass / (b->mass + neighbor->mass) * vec_dot(temp, normal);
                    vec_kill(temp);
                    vector *delta = vec_mul(normal, n / b->mass);
                    vec_kill(normal);
                    vector *old = *new_velocity;
                    *new_velocity = vec_add(old, delta);
                    vec_kill(old);
                    vec_kill(delta);
                }
            }
            vec_kill(d_vec);
        }
        lst_kill_iter(it);
    }
    else {
        for (int i = -1; i < 2; i++) {
            int cand = b->block_index[cur] + i;
            if (cand < 0 || cand >= r->block_num[cur]) {
                continue;
            }
            index[cur] = cand;
            check_neighbor_helper(r, b, new_velocity, interval, cur + 1, index);
        }
    }
}

static void check_neighbor(room *r, ball *b, vector **new_velocity, int interval) {
    int *index = (int *)malloc(sizeof(int) * r->dim);
    check_neighbor_helper(r, b, new_velocity, interval, 0, index);
    free(index);
}

static void calculate_velocity(room *r, ball *b, int interval) {
    b->new_velocity = vec_copy(b->velocity);
    // border
    check_border(r, b, &b->new_velocity);
    // other balls
    check_neighbor(r, b, &b->new_velocity, interval);
}

static void update_velocity(ball *b) {
    vec_kill(b->velocity);
    b->velocity = b->new_velocity;
}

static void update_position(ball *b, int interval) {
    vector *old_position = b->position;
    b->position = next_position(b, interval);
    vec_kill(old_position);
}

static void update_block(room *r, ball *b, list_node *n) {
    if (get_block_index(r, b)) {
        lst_remove(n);
        list *l;
        nda_get(r->block, b->block_index, &l);
        lst_add_tail(l, NULL, n);
    }
}

static void fill_circle(SDL_Renderer *renderer, int cx, int cy, int r) {
    int mx = cx + r;
    int my = cy + r;
    for (int x = cx - r; x <= mx; x++) {
        for (int y = cy - r; y <= my; y++) {
            if ((x - cx) * (x - cx) + (y - cy) * (y - cy) <= r * r) {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}

static void draw_circle(SDL_Renderer *renderer, int cx, int cy, double r) {
    double delta = 1.0 / r;
    double theta = 0;
    while (theta < 2 * M_PI) {
        int x = cx + (int)round(r * cos(theta));
        int y = cy + (int)round(r * sin(theta));
        SDL_RenderDrawPoint(renderer, x, y);
        theta += delta;
    }
}

static void run(room *r, int interval, bool show, SDL_Renderer *renderer, double scale, bool fill) {
    list_iter *it = lst_init_iter(r->ball_list, false);
    list_node *n;
    // calculate new velocity
    while (lst_iter_next(it, &n)) {
        ball *b;
        lst_get_node(n, &b);
        calculate_velocity(r, b, interval);
    }
    // update velocity
    lst_iter_reset(it);
    while (lst_iter_next(it, &n)) {
        ball *b;
        lst_get_node(n, &b);
        update_velocity(b);
    }
    // update position and the block belonging to
    lst_iter_reset(it);
    while (lst_iter_next(it, &n)) {
        ball *b;
        lst_get_node(n, &b);
        update_position(b, interval);
        update_block(r, b, n);
    }
    if (show) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black background
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white balls
        lst_iter_reset(it);
        while (lst_iter_next(it, &n)) {
            ball *b;
            lst_get_node(n, &b);
            int x = (int)round((*vec_entry(b->position, 1) - r->range_min[0]) * scale);
            int y = (int)round((r->range_max[1] - *vec_entry(b->position, 2)) * scale);
            if (fill) {
                int r = (int)round(b->radius * scale);
                fill_circle(renderer, x, y, r);
            }
            else {
                double r = b->radius * scale;
                draw_circle(renderer, x, y, r);
            }
        }
        SDL_RenderPresent(renderer);
    }
    lst_kill_iter(it);
}

static Uint32 callback(Uint32 interval, void *arg) {
    int *rounds = (int *)arg;
    SDL_Event event;
    if (*rounds == 0) {
        event.type = SDL_QUIT;
        interval = 0;
    }
    else {
        event.type = SDL_USEREVENT;
    }
    SDL_PushEvent(&event);
    if (*rounds > 0) {
        (*rounds)--;
    }
    return interval;
}

void cls_start(room *r, int interval, int rounds) {
    assert(interval > 0);
    while (rounds < 0 || rounds--) {
        run(r, interval, false, NULL, 0, false);
    }
}

void cls_start_show(room *r, int interval, int rounds, int fps, bool fill) {
    assert(interval > 0);
    assert(r->dim == 2);
    assert(fps > 0);
    if (fps > MAX_FPS) {
        fprintf(stderr, "[warning] cls_start_show: high fps may crash your computer\n");
    }
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);
    double scale_cand1 = (double)dm.w * WINDOW_TO_DISPLAY_RATIO / (r->range_max[0] - r->range_min[0]);
    double scale_cand2 = (double)dm.h * WINDOW_TO_DISPLAY_RATIO / (r->range_max[1] - r->range_min[1]);
    double scale = fmin(scale_cand1, scale_cand2);
    int width = (int)round((r->range_max[0] - r->range_min[0]) * scale);
    int height = (int)round((r->range_max[1] - r->range_min[1]) * scale);
    SDL_SetWindowSize(window, width, height);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_TimerID timer_id = SDL_AddTimer(1000 / fps, callback, &rounds);
    SDL_Event event;
    while (true) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_USEREVENT) {
            run(r, interval, true, renderer, scale, fill);
        }
        else if (event.type == SDL_QUIT) {
            break;
        }
    }
    SDL_RemoveTimer(timer_id);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

static void kill_block_helper(ndarray *block, int *size, int dim, int cur, int *index) {
    if (cur == dim) {
        list *l;
        nda_get(block, index, &l);
        lst_kill(l);
    }
    else {
        for (int i = 0; i < size[cur]; i++) {
            index[cur] = i;
            kill_block_helper(block, size, dim, cur + 1, index);
        }
    }
}

static void kill_block(ndarray *block, int *size, int dim) {
    int *index = (int *)malloc(sizeof(int) * dim);
    kill_block_helper(block, size, dim, 0, index);
    free(index);
}

void cls_kill_room(room *r) {
    free(r->range_min);
    free(r->range_max);
    kill_block(r->block, r->block_num, r->dim);
    free(r->block_num);
    list_iter *it = lst_init_iter(r->ball_list, false);
    list_node *n;
    while (lst_iter_next(it, &n)) {
        ball *b;
        lst_get_node(n, &b);
        cls_kill_ball(b);
        lst_kill_node(n);
    }
    lst_kill_iter(it);
    lst_kill(r->ball_list);
    free(r);
}

void cls_kill_ball(ball *b) {
    vec_kill(b->position);
    vec_kill(b->velocity);
    free(b->block_index);
    free(b);
}
