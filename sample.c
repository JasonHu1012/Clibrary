#include "matrix.h"
#include "list.h"
#include "stack.h"
#include "queue.h"
#include "vector.h"
#include "ndarray.h"
#include "func.h"
#include "collision.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void matrix_sample() {
    printf("---matrix---\n");
    matrix *a = mat_random(3, 4, 1.5, 9.5, false);
    matrix *b = mat_random(4, 2, 2.4, 10.9, true);
    matrix *m = mat_multiply(a, b);
    mat_print(m);
    printf("%f\n", *mat_entry(m, 2, 2));
    *mat_entry(m, 2, 2) = 100;
    mat_print(m);
    mat_kill(a);
    mat_kill(b);
    mat_kill(m);
}

void list_sample() {
    printf("---list---\n");
    list *l = lst_init(sizeof(int));
    int i = 100;
    lst_add_head(l, &i, NULL);
    i = 200;
    lst_add_head(l, &i, NULL);
    i = 300;
    lst_add_tail(l, &i, NULL);
    // 200 100 300
    list_iter *it = lst_init_iter(l, false);
    printf("list content:\n");
    while (lst_iter_next(it, &i)) {
        printf("%d ", i);
    }
    printf("\n");
    lst_iter_reset(it);
    lst_iter_next(it, NULL);
    lst_iter_next(it, NULL);
    lst_iter_remove(it);
    printf("remove 100\n");
    list_iter *it2 = lst_init_iter(l, true);
    printf("list content (reverse):\n");
    while (lst_iter_next(it2, &i)) {
        printf("%d ", i);
    }
    printf("\n");
    lst_kill_iter(it2);
    printf("list size: %d\n", lst_size(l));
    i = 50;
    list_node *n = lst_init_node(&i, sizeof(int));
    lst_iter_next(it, &i);
    printf("iter current at: %d\n", i);
    lst_add_iter_prev(it, NULL, n);
    printf("add 50 before 300\n");
    lst_iter_reset(it);
    printf("list content:\n");
    while (lst_iter_next(it, &i)) {
        printf("%d ", i);
    }
    printf("\n");
    lst_kill_iter(it);
    lst_kill(l);
    lst_kill_node(n);
}

void stack_sample() {
    printf("---stack---\n");
    stack *s = stk_init(sizeof(int));
    int i = 100;
    stk_push(s, &i);
    i = 200;
    stk_push(s, &i);
    i = 0;
    stk_top(s, &i);
    printf("%d\n", i); // 200
    stk_pop(s, NULL);
    printf("%d\n", stk_size(s)); // 1
    stk_pop(s, &i);
    printf("%d\n", i); // 100
    stk_kill(s);
}

void queue_sample() {
    printf("---queue---\n");
    queue *q = que_init(sizeof(int));
    int i = 100;
    que_push(q, &i);
    i = 200;
    que_push(q, &i);
    i = 0;
    que_top(q, &i);
    printf("%d\n", i); // 100
    que_pop(q, NULL);
    printf("%d\n", que_size(q)); // 1
    que_pop(q, &i);
    printf("%d\n", i); // 200
    que_kill(q);
}

void vector_sample() {
    printf("---vector---\n");
    int iarr[3] = {0, 3, 5};
    vector *iv = vec_iarr(iarr, 3);
    double darr[3] = {5.2, 0.3, 5.0};
    vector *dv = vec_darr(darr, 3);
    vector *v = vec_add(iv, dv);
    vector *pv = vec_project(iv, dv);
    vec_print(pv);
    vec_kill(pv);
    vec_print(v);
    *vec_entry(v, 1) = 1000;
    vec_print(v);
    vec_kill(v);
    printf("%f\n", vec_dot(iv, dv)); // 25.9
    printf("%f\n", vec_len(iv)); // 5.83
    vec_kill(iv);
    vec_kill(dv);
}

void ndarray_sample() {
    printf("---ndarray---\n");
    int dim = rand() % 5 + 1;
    printf("dimension: %d\n", dim);
    int *size = (int *)malloc(sizeof(int) * dim);
    for (int i = 0; i < dim; i++) {
        size[i] = 5;
    }
    ndarray *a = nda_init(size, dim, sizeof(int));
    int i = 100;
    printf("index:");
    int *index = (int *)malloc(sizeof(int) * dim);
    for (int d = 0; d < dim; d++) {
        index[d] = rand() % 5;
        printf(" %d", index[d]);
    }
    printf("\n");
    nda_set(a, index, &i);
    i = 0;
    nda_get(a, index, &i);
    printf("%d\n", i); // 100
    free(size);
    free(index);
    nda_kill(a);
}

void func_sample() {
    printf("---func---\n");
    int len[3] = {5, 5, 5};
    int ***arr = (int ***)salloc(sizeof(int), len, 3);
    int n = 100;
    printf("n = 100;\nn: %d\n", n);
    arr[2][3][4] = n;
    printf("arr[2][3][4] = n;\narr[2][3][4]: %d\n", arr[2][3][4]);
    n = 0;
    printf("n = 0;\nn: %d\n", n);
    n = arr[2][3][4];
    printf("n = arr[2][3][4];\nn: %d\n", n);
    free(arr);
    printf("%d\n", randi(5, 10));
    printf("%f\n", randf(3.5, 5.2));
    if (str_to_num("123", &n)) {
        printf("%d\n", n);
    }
    int *dr = distinct_rand(1, 100, 10);
    for (int i = 0; i < 10; i++) {
        printf("%d ", dr[i]);
    }
    printf("\n");
}

void collision_sample() {
    printf("---collision---\n");
    room *r = cls_init_room(2, (double[]){-5, -5}, (double[]){5, 5}, 1);
    for (int i = 0; i < 4; i++) {
        vector *position = vec_iarr((int[]){i % 2 * 4 - 2, i / 2 * -4 + 2}, 2);
        vector *velocity = vec_random(2, -10, 10, false);
        ball *b = cls_init_ball(1, rand() % 5 + 1, position, velocity);
        vec_kill(position);
        vec_kill(velocity);
        cls_add_ball(r, b);
        cls_kill_ball(b);
    }
    cls_print(r);
    printf("%f\n", cls_kinetic(r));
    cls_start(r, 5000, 10000);
    cls_start_show(r, 20000, 300, 50, false);
    printf("%f\n", cls_kinetic(r));
    cls_kill_room(r);
}

int main() {
    srand(time(NULL));
    matrix_sample();
    list_sample();
    stack_sample();
    queue_sample();
    vector_sample();
    ndarray_sample();
    func_sample();
    collision_sample();
    return 0;
}
