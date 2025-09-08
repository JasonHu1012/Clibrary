#ifndef _VECTOR_H
#define _VECTOR_H

typedef struct vector vector;

// for all functions that generate results to `dst`, `dst` can be one of input arguments
// `dst` could be initialized vector with correct dimension, and the functions will return it
// `dst` could be NULL, and the functions will initialize a new vector and return it

// initialize a zero vector with dimension `dim`
vector *vec_zero(int dim);

// initialize a vector from `arr` with dimension `dim`
vector *vec_arr(int dim, double *arr);

// kill `vec`
void vec_kill(vector *vec);

// return the pointer of the `index`-th entry of `vec` to get and set it
double *vec_entry(vector *vec, int index);

// return the length (2-norm) of `vec`
double vec_len(vector *vec);

// return the dimension of `vec`
int vec_dim(vector *vec);

// return the inner product of `vec1` and `vec2`
double vec_dot(vector *vec1, vector *vec2);

// let `dst` be `vec1` added by `vec2`
vector *vec_add(vector *vec1, vector *vec2, vector *dst);

// let `dst` be `vec1` subtracted by `vec2`
vector *vec_minus(vector *vec1, vector *vec2, vector *dst);

// let `dst` be `vec` multiplied by `mul`
vector *vec_mul(vector *vec, double mul, vector *dst);

// let `dst` be the unit vector of `vec`
vector *vec_unit(vector *vec, vector *dst);

// let `dst` be the projection of `vec1` onto `vec2`
vector *vec_proj(vector *vec1, vector *vec2, vector *dst);

#endif
