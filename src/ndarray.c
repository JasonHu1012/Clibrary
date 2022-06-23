#include "ndarray.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct _ndarray _ndarray;

struct _ndarray {
    _ndarray **content;
    int size_or_width;
};

struct ndarray {
    _ndarray **content;
    int size;
    int dim;
};

static _ndarray *init(int *size, int dim, int width) {
    _ndarray *output = (_ndarray *)malloc(sizeof(_ndarray));
    if (dim) {
        output->content = (_ndarray **)malloc(sizeof(_ndarray *) * size[0]);
        for (int i = 0; i < size[0]; i++) {
            output->content[i] = init(size + 1, dim - 1, width);
        }
        output->size_or_width = size[0];
    }
    else {
        output->content = (_ndarray **)malloc(width);
        output->size_or_width = width;
    }
    return output;
}

ndarray *nda_init(int *size, int dim, int width) {
    assert(dim > 0 && width > 0);
    for (int i = 0; i < dim; i++) {
        assert(size[i] > 0);
    }
    ndarray *output = (ndarray *)malloc(sizeof(ndarray));
    output->content = (_ndarray **)malloc(sizeof(_ndarray *) * size[0]);
    for (int i = 0; i < size[0]; i ++) {
        output->content[i] = init(size + 1, dim - 1, width);
    }
    output->size = size[0];
    output->dim = dim;
    return output;
}

void nda_get(ndarray *a, int *index, void *dst) {
    assert(index[0] >= 0 && index[0] < a->size);
    _ndarray *cur = a->content[index[0]];
    for (int d = 1; d < a->dim; d++) {
        assert(index[d] >= 0 && index[d] < cur->size_or_width);
        cur = cur->content[index[d]];
    }
    memcpy(dst, cur->content, cur->size_or_width);
}

void nda_set(ndarray *a, int *index, void *src) {
    assert(index[0] >= 0 && index[0] < a->size);
    _ndarray *cur = a->content[index[0]];
    for (int d = 1; d < a->dim; d++) {
        assert(index[d] >= 0 && index[d] < cur->size_or_width);
        cur = cur->content[index[d]];
    }
    memcpy(cur->content, src, cur->size_or_width);
}

static void kill(_ndarray *a, int dim) {
    if (dim) {
        for (int i = 0; i < a->size_or_width; i++) {
            kill(a->content[i], dim - 1);
        }
    }
    free(a->content);
    free(a);
}

void nda_kill(ndarray *a) {
    for (int i = 0; i < a->size; i++) {
        kill(a->content[i], a->dim - 1);
    }
    free(a->content);
    free(a);
}
