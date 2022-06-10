#include "vector.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>

struct vector {
    int dim;
    double *entry;
};

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

#define VEC_FROM_ARR\
    assert(dim > 0);\
    vector *output = (vector *)malloc(sizeof(vector));\
    output->dim = dim;\
    output->entry = (double *)malloc(sizeof(double) * dim);\
    for (int i = 0; i < dim; i++) {\
        output->entry[i] = arr[i];\
    }\
    return output;

vector *vec_iarr(int *arr, int dim) {
    VEC_FROM_ARR
}

vector *vec_darr(double *arr, int dim) {
    VEC_FROM_ARR
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

double vec_len(vector *v) {
    double output = 0;
    for (int i = 0; i < v->dim; i++) {
        output += v->entry[i] * v->entry[i];
    }
    return sqrt(output);
}

void vec_print(vector *v) {
    printf("%.2e", v->entry[0]);
    int dim = v->dim;
    for (int i = 1; i < dim; i++) {
        printf(" %.2e", v->entry[i]);
    }
    printf("\n");
}

vector *vec_copy(vector *v) {
    vector *output = (vector *)malloc(sizeof(vector));
    int dim = v->dim;
    output->dim = dim;
    output->entry = (double *)malloc(sizeof(double) * dim);
    memcpy(output->entry, v->entry, sizeof(double) * dim);
    return output;
}

void vec_kill(vector *v) {
    free(v->entry);
    free(v);
}
