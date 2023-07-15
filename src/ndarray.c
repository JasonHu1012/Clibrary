#include "ndarray.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    struct node **data;
    int size;
} node;

struct ndarray {
    node *data;
    int width;
    int dim;
};

static node *init_node(int *sizes, int dim_i, int dim, int width) {
    node *ret = (node *)malloc(sizeof(node));

    if (dim_i < dim - 1) {
        ret->data = (node **)malloc(sizeof(node *) * sizes[dim_i]);
        for (int i = 0; i < sizes[dim_i]; i++) {
            ret->data[i] = init_node(sizes, dim_i + 1, dim, width);
        }
    }
    else {
        ret->data = (node **)malloc(width * sizes[dim_i]);
    }

    ret->size = sizes[dim_i];
    return ret;
}

static void kill_node(node *n, int dim_i, int dim) {
    if (dim_i < dim - 1) {
        for (int i = 0; i < n->size; i++) {
            kill_node(n->data[i], dim_i + 1, dim);
        }
    }

    free(n->data);
    free(n);
}

ndarray *nda_init(int *sizes, int dim, int width) {
    assert(dim > 0 && width > 0);
    for (int i = 0; i < dim; i++) {
        assert(sizes[i] > 0);
    }

    ndarray *ret = (ndarray *)malloc(sizeof(ndarray));
    ret->data = init_node(sizes, 0, dim, width);
    ret->width = width;
    ret->dim = dim;
    return ret;
}

void nda_kill(ndarray *nda) {
    kill_node(nda->data, 0, nda->dim);
    free(nda);
}

void nda_get(ndarray *nda, int *indexes, void *dst) {
    // because size is stored in node, can't check argument validity first
    node *cur = nda->data;
    for (int d = 0; d < nda->dim - 1; d++) {
        assert(indexes[d] >= 0 && indexes[d] < cur->size);

        cur = cur->data[indexes[d]];
    }

    assert(indexes[nda->dim - 1] >= 0 && indexes[nda->dim - 1] < cur->size);

    memcpy(
        dst,
        (char *)cur->data + nda->width * indexes[nda->dim - 1],
        nda->width);
}

void nda_set(ndarray *nda, int *indexes, void *src) {
    // because size is stored in node, can't check argument validity first
    node *cur = nda->data;
    for (int d = 0; d < nda->dim - 1; d++) {
        assert(indexes[d] >= 0 && indexes[d] < cur->size);

        cur = cur->data[indexes[d]];
    }

    assert(indexes[nda->dim - 1] >= 0 && indexes[nda->dim - 1] < cur->size);

    memcpy(
        (char *)cur->data + nda->width * indexes[nda->dim - 1],
        src,
        nda->width);
}
