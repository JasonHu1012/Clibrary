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

double vec_dot(vector *vec1, vector *vec2) {
    assert(vec1->dim == vec2->dim);

    double ret = 0.;
    for (int i = 0; i < vec1->dim; i++) {
        ret += vec1->entry[i] * vec2->entry[i];
    }
    return ret;
}

vector *vec_add(vector *vec1, vector *vec2, vector *dst) {
    assert(vec1->dim == vec2->dim);
    if (dst) {
        assert(vec1->dim == dst->dim);
    }

    if (!dst) {
        dst = vec_zero(vec1->dim);
    }

    for (int i = 0; i < vec1->dim; i++) {
        dst->entry[i] = vec1->entry[i] + vec2->entry[i];
    }
    return dst;
}

vector *vec_minus(vector *vec1, vector *vec2, vector *dst) {
    assert(vec1->dim == vec2->dim);
    if (dst) {
        assert(vec1->dim == dst->dim);
    }

    if (!dst) {
        dst = vec_zero(vec1->dim);
    }

    for (int i = 0; i < vec1->dim; i++) {
        dst->entry[i] = vec1->entry[i] - vec2->entry[i];
    }
    return dst;
}

vector *vec_mul(vector *vec, double mul, vector *dst) {
    if (dst) {
        assert(vec->dim == dst->dim);
    }

    if (!dst) {
        dst = vec_zero(vec->dim);
    }

    for (int i = 0; i < vec->dim; i++) {
        dst->entry[i] = vec->entry[i] * mul;
    }
    return dst;
}

vector *vec_unit(vector *vec, vector *dst) {
    if (dst) {
        assert(vec->dim == dst->dim);
    }

    if (!dst) {
        dst = vec_zero(vec->dim);
    }

    int len = vec_len(vec);
    for (int i = 0; i < vec->dim; i++) {
        dst->entry[i] = vec->entry[i] / len;
    }
    return dst;
}

vector *vec_proj(vector *vec1, vector *vec2, vector *dst) {
    assert(vec1->dim == vec2->dim);
    if (dst) {
        assert(vec1->dim == dst->dim);
    }

    if (!dst) {
        dst = vec_zero(vec1->dim);
    }

    int vec2_len = vec_len(vec2);
    return vec_mul(vec2, vec_dot(vec1, vec2) / (vec2_len * vec2_len), dst);
}
