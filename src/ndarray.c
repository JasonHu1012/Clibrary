#include "ndarray.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    struct node **data;
} node;

struct ndarray {
    int *sizes;
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

    return ret;
}

static void kill_node(node *n, int dim_i, int dim, int *sizes) {
    if (dim_i < dim - 1) {
        for (int i = 0; i < sizes[dim_i]; i++) {
            kill_node(n->data[i], dim_i + 1, dim, sizes);
        }
    }

    free(n->data);
    free(n);
}

static void kill_node_f(node *n, int dim_i, int dim, int *sizes, void (*kill)(void *ptr)) {
    for (int i = 0; i < sizes[dim_i]; i++) {
        if (dim_i < dim - 1) {
            kill_node_f(n->data[i], dim_i + 1, dim, sizes, kill);
        }
        else {
            kill(((void **)n->data)[i]);
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
    ret->sizes = (int *)malloc(sizeof(int) * dim);
    memcpy(ret->sizes, sizes, sizeof(int) * dim);
    ret->data = init_node(ret->sizes, 0, dim, width);
    ret->width = width;
    ret->dim = dim;
    return ret;
}

void nda_kill(void *ptr) {
    if (!ptr) {
        return;
    }

    ndarray *nda = (ndarray *)ptr;
    kill_node(nda->data, 0, nda->dim, nda->sizes);
    free(nda->sizes);
    free(nda);
}

void nda_kill_f(ndarray *nda, void (*kill)(void *ptr)) {
    if (!nda) {
        return;
    }

    kill_node_f(nda->data, 0, nda->dim, nda->sizes, kill);
    free(nda->sizes);
    free(nda);
}

void nda_get(ndarray *nda, int *indexes, void *dst) {
    for (int d = 0; d < nda->dim; d++) {
        assert(indexes[d] >= 0 && indexes[d] < nda->sizes[d]);
    }

    node *cur = nda->data;
    for (int d = 0; d < nda->dim - 1; d++) {
        cur = cur->data[indexes[d]];
    }

    memcpy(
        dst,
        (char *)cur->data + nda->width * indexes[nda->dim - 1],
        nda->width);
}

void nda_set(ndarray *nda, int *indexes, void *src) {
    for (int d = 0; d < nda->dim; d++) {
        assert(indexes[d] >= 0 && indexes[d] < nda->sizes[d]);
    }

    node *cur = nda->data;
    for (int d = 0; d < nda->dim - 1; d++) {
        cur = cur->data[indexes[d]];
    }

    memcpy(
        (char *)cur->data + nda->width * indexes[nda->dim - 1],
        src,
        nda->width);
}
