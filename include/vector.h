#ifndef _VECTOR_H
#define _VECTOR_H

typedef struct vector vector;

// for all functions that generate results to `dst`, `dst` can be one of input arguments
// `dst` should have been initialized, and its dimension should be correct

// initialize a zero vector with dimension `dim`
vector *vec_zero(int dim);

// initialize a vector with dimension `dim`
// each entry is randomly generated from [`min`, `max`]
vector *vec_rand(int dim, double min, double max);

// initialize a vector with dimension `dim` whose entries are equal to `arr`
vector *vec_arr(int dim, double *arr);

// kill `vec`
void vec_kill(vector *vec);

// return the pointer of the `index`-th entry of `vec` to get and set it
double *vec_entry(vector *vec, int index);

// return the length (2-norm) of `vec`
double vec_len(vector *vec);

// return the dimension of `vec`
int vec_dim(vector *vec);

// let `dst` = `vec1` + `vec2`
void vec_add(vector *vec1, vector *vec2, vector *dst);

// let `dst` = `vec1` - `vec2`
void vec_minus(vector *vec1, vector *vec2, vector *dst);

// return the inner product of `vec1` and `vec2`
double vec_dot(vector *vec1, vector *vec2);

// let `dst` be `mul` * `vec`
void vec_mul(vector *vec, double mul, vector *dst);

// let `dst` be the unit vector of `vec`
void vec_unit(vector *vec, vector *dst);

// let `dst` be the projection of `vec1` onto `vec2`
void vec_proj(vector *vec1, vector *vec2, vector *dst);

#endif
