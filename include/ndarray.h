#ifndef _NDARRAY_H
#define _NDARRAY_H

typedef struct ndarray ndarray;

// initialize an ndarray with dimension `dim` of type occupying `width` byte(s)
// `sizes`[i] represents the size of the i-th dimension
ndarray *nda_init(int *sizes, int dim, int width);

// kill `nda`
void nda_kill(ndarray *nda);

// copy `nda[indexes]` to `dst`
// `indexes` is an array whose length is the dimension of `nda`
// `indexes`[i] represents the index of the i-th dimension
void nda_get(ndarray *nda, int *indexes, void *dst);

// copy `src` to `nda[indexes]`
// `indexes` is an array whose length is the dimension of `nda`
// `indexes`[i] represents the index of the i-th dimension
void nda_set(ndarray *nda, int *indexes, void *src);

#endif
