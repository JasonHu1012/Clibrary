#include "vector.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

vector *vec_zero(int dim) {
    assert(dim > 0);
    vector *output = (vector *)malloc(sizeof(vector));
    output->dim = dim;
    output->entry = (double *)calloc(dim, sizeof(double));
    return output;
}

vector *vec_random(int dim, double min, double max, bool integer) {
    assert(dim > 0);
    assert(min <= max);
    vector *output = (vector *)malloc(sizeof(vector));
    output->dim = dim;
    output->entry = (double *)malloc(sizeof(double) * dim);
    for (int i = 0; i < dim; i++) {
        output->entry[i] = integer ? rand() % ((int)max - (int)min + 1) + (int)min : (max - min) * rand() / RAND_MAX + min;
    }
    return output;
}

vector *vec_add(vector *a, vector *b) {
    assert(a->dim == b->dim);
    vector *output = (vector *)malloc(sizeof(vector));
    int dim = a->dim;
    output->dim = dim;
    output->entry = (double *)malloc(sizeof(double) * dim);
    for (int i = 0; i < dim; i++) {
        output->entry[i] = a->entry[i] + b->entry[i];
    }
    return output;
}

vector *vec_minus(vector *a, vector *b) {
    assert(a->dim == b->dim);
    vector *output = (vector *)malloc(sizeof(vector));
    int dim = a->dim;
    output->dim = dim;
    output->entry = (double *)malloc(sizeof(double) * dim);
    for (int i = 0; i < dim; i++) {
        output->entry[i] = a->entry[i] - b->entry[i];
    }
    return output;
}

vector *vec_mul(vector *v, double n) {
    vector *output = (vector *)malloc(sizeof(vector));
    int dim = v->dim;
    output->dim = dim;
    output->entry = (double *)malloc(sizeof(double) * dim);
    for (int i = 0; i < dim; i++) {
        output->entry[i] = n * v->entry[i];
    }
    return output;
}

double vec_dot(vector *a, vector *b) {
    assert(a->dim == b->dim);
    double output = 0;
    int dim = a->dim;
    for (int i = 0; i < dim; i++) {
        output += a->entry[i] * b->entry[i];
    }
    return output;
}

double *vec_entry(vector *v, int index) {
    assert(index >= 1 && index <= v->dim);
    return &v->entry[index - 1];
}

void vec_print(vector *v) {
    printf("%.2e", v->entry[0]);
    int dim = v->dim;
    for (int i = 1; i < dim; i++) {
        printf(" %.2e", v->entry[i]);
    }
    printf("\n");
}

void vec_kill(vector *v) {
    free(v->entry);
    free(v);
}
