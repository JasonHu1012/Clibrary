#include "matrix.h"
#include "list.h"
#include "stack.h"
#include "queue.h"
#include "vector.h"
#include "ndarray.h"
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
    mat_kill(m);
}

void list_sample() {
    printf("---list---\n");
    list *l = lst_init(sizeof(int));
    int i = 30;
    lst_append(l, &i, NULL);
    i = 50;
    list_node *n = lst_init_node(&i, sizeof(int));
    lst_append(l, NULL, n);
    lst_iter_init(l, false);
    i = 0;
    while (lst_iter(l, &i)) {
        printf("%d\n", i); // 30 50
    }
    lst_remove(l, n, true);
    printf("%d\n", lst_size(l)); // 1
    lst_kill(l);
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
    vec_print(v);
    *vec_entry(v, 1) = 1000;
    vec_print(v);
    vec_kill(v);
    printf("%f\n", vec_dot(iv, dv)); // 25.9
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
    nda_kill(a);
}

int main() {
    srand(time(NULL));
    matrix_sample();
    list_sample();
    stack_sample();
    queue_sample();
    vector_sample();
    ndarray_sample();
    return 0;
}
