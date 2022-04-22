#ifndef _NDARRAY_H
#define _NDARRAY_H

typedef struct ndarray ndarray;

ndarray *nda_init(int *size, int dim, int width);
void nda_get(ndarray *a, int *index, void *dst);
void nda_set(ndarray *a, int *index, void *src);
void nda_kill(ndarray *a);

typedef struct _ndarray _ndarray;

struct ndarray {
    _ndarray **content;
    int size;
    int dim;
};

#endif
