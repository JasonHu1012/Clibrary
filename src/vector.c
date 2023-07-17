#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

struct vector {
    double *entry;
    int dim;
};

vector *vec_zero(int dim) {
    assert(dim > 0);

    vector *ret = (vector *)malloc(sizeof(vector));
    ret->entry = (double *)calloc(dim, sizeof(double));
    ret->dim = dim;
    return ret;
}

static double rand_double(double min, double max) {
    return (double)rand() / RAND_MAX * (max - min) + min;
}

vector *vec_rand(int dim, double min, double max) {
    assert(dim > 0);
    assert(max >= min);

    vector *ret = (vector *)malloc(sizeof(vector));
    ret->entry = (double *)malloc(sizeof(double) * dim);
    for (int i = 0; i < dim; i++) {
        ret->entry[i] = rand_double(min, max);
    }
    ret->dim = dim;
    return ret;
}

vector *vec_arr(int dim, double *arr) {
    assert(dim > 0);

    vector *ret = (vector *)malloc(sizeof(vector));
    ret->entry = (double *)malloc(sizeof(double) * dim);
    memcpy(ret->entry, arr, sizeof(double) * dim);
    ret->dim = dim;
    return ret;
}

void vec_kill(vector *vec) {
    free(vec->entry);
    free(vec);
}

double *vec_entry(vector *vec, int index) {
    assert(index >= 0 && index < vec->dim);

    return &vec->entry[index];
}

double vec_len(vector *vec) {
    double ret = 0.;
    for (int i = 0; i < vec->dim; i++) {
        ret += vec->entry[i] * vec->entry[i];
    }
    return sqrt(ret);
}

int vec_dim(vector *vec) {
    return vec->dim;
}

void vec_add(vector *vec1, vector *vec2, vector *dst) {
    assert(vec1->dim == vec2->dim);
    assert(vec1->dim == dst->dim);

    for (int i = 0; i < vec1->dim; i++) {
        dst->entry[i] = vec1->entry[i] + vec2->entry[i];
    }
}

void vec_minus(vector *vec1, vector *vec2, vector *dst) {
    assert(vec1->dim == vec2->dim);
    assert(vec1->dim == dst->dim);

    for (int i = 0; i < vec1->dim; i++) {
        dst->entry[i] = vec1->entry[i] - vec2->entry[i];
    }
}

double vec_dot(vector *vec1, vector *vec2) {
    assert(vec1->dim == vec2->dim);

    double ret = 0.;
    for (int i = 0; i < vec1->dim; i++) {
        ret += vec1->entry[i] * vec2->entry[i];
    }
    return ret;
}

void vec_mul(vector *vec, double mul, vector *dst) {
    assert(vec->dim == dst->dim);

    for (int i = 0; i < vec->dim; i++) {
        dst->entry[i] = vec->entry[i] * mul;
    }
}

void vec_unit(vector *vec, vector *dst) {
    assert(vec->dim == dst->dim);

    int len = vec_len(vec);
    for (int i = 0; i < vec->dim; i++) {
        dst->entry[i] = vec->entry[i] / len;
    }
}

void vec_proj(vector *vec1, vector *vec2, vector *dst) {
    assert(vec1->dim == vec2->dim);
    assert(vec1->dim == dst->dim);

    int vec2_len = vec_len(vec2);
    vec_mul(vec2, vec_dot(vec1, vec2) / vec2_len * vec2_len, dst);
}
