#include "collision.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

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

static void check_border(room *r, ball *b, vector **new_velocity) {
    for (int i = 0; i < r->dim; i++) {
        vector *normal = vec_zero(r->dim);
        if (*vec_entry(b->position, i + 1) - b->radius < r->range_min[i]) {
            *vec_entry(normal, i + 1) = 1;
        }
        else if (*vec_entry(b->position, i + 1) + b->radius > r->range_max[i]) {
            *vec_entry(normal, i + 1) = -1;
        }
        if (vec_len(normal)) {
            vector *proj = vec_project(b->velocity, normal);
            vector *delta = vec_mul(proj, -2);
            vec_kill(proj);
            vector *old = *new_velocity;
            *new_velocity = vec_add(old, delta);
            vec_kill(old);
            vec_kill(delta);
        }
        vec_kill(normal);
    }
}

static void check_neighbor_helper(room *r, ball *b, vector **new_velocity, int cur, int *index) {
    if (cur == r->dim) {
        list *l;
        nda_get(r->block, index, &l);
        list_iter *it = lst_init_iter(l, false);
        ball *neighbor;
        while (lst_iter_next(it, &neighbor)) {
            if (neighbor == b) {
                continue;
            }
            vector *distance = vec_minus(b->position, neighbor->position);
            if (vec_len(distance) < b->radius + neighbor->radius) {
                vector *normal = vec_unit(distance);
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
            vec_kill(distance);
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
            check_neighbor_helper(r, b, new_velocity, cur + 1, index);
        }
    }
}

static void check_neighbor(room *r, ball *b, vector **new_velocity) {
    int *index = (int *)malloc(sizeof(int) * r->dim);
    check_neighbor_helper(r, b, new_velocity, 0, index);
    free(index);
}

static void calculate_velocity(room *r, ball *b) {
    b->new_velocity = vec_copy(b->velocity);
    // border
    check_border(r, b, &b->new_velocity);
    // other balls
    check_neighbor(r, b, &b->new_velocity);
}

static void update_velocity(ball *b) {
    vec_kill(b->velocity);
    b->velocity = b->new_velocity;
}

static void update_position(ball *b, int interval) {
    vector *old_position = b->position;
    vector *delta = vec_mul(b->velocity, (double)interval / 1000000);
    b->position = vec_add(old_position, delta);
    vec_kill(old_position);
    vec_kill(delta);
}

static void update_block(room *r, ball *b, list_node *n) {
    if (get_block_index(r, b)) {
        lst_remove(n);
        list *l;
        nda_get(r->block, b->block_index, &l);
        lst_add_tail(l, NULL, n);
    }
}

static double kinetic(room *r) {
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

void cls_start(room *r, int interval, int round) {
    assert(interval > 0);
    while (round < 0 || round--) {
        printf("%f\n", kinetic(r));
        list_iter *it = lst_init_iter(r->ball_list, false);
        list_node *n;
        // calculate new velocity
        while (lst_iter_next(it, &n)) {
            ball *b;
            lst_get_node(n, &b);
            calculate_velocity(r, b);
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
        printf("%f\n", kinetic(r));
    }
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
