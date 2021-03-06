#ifndef _VECTOR_H
#define _VECTOR_H

#include <stdbool.h>

typedef struct vector vector;

vector *vec_zero(int dim);
vector *vec_random(int dim, double min, double max, bool integer);
// build vector from int/double array
vector *vec_iarr(int *arr, int dim);
vector *vec_darr(double *arr, int dim);
vector *vec_add(vector *a, vector *b);
vector *vec_minus(vector *a, vector *b);
vector *vec_mul(vector *v, double n);
vector *vec_copy(vector *v);
vector *vec_unit(vector *v);
// project a onto b
vector *vec_project(vector *a, vector *b);
double vec_dot(vector *a, vector *b);
// 1-based; return the pointer of the entry
double *vec_entry(vector *v, int index);
double vec_len(vector *v);
int vec_dim(vector *v);
void vec_print(vector *v);
void vec_kill(vector *v);

#endif
