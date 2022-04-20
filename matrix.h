#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>

typedef struct matrix matrix;

matrix *mat_random(int row_size, int col_size, double min, double max, bool integer);
matrix *mat_zero(int row_size, int col_size);
matrix *mat_one(int row_size, int col_size);
matrix *mat_identity(int size);
matrix *mat_transpose(matrix *m);
matrix *mat_read(int row_size, int col_size);
void mat_print(matrix *m);
void mat_kill(matrix *m);
// 1-based; return the pointer of the entry
double *mat_entry(matrix *m, int row_index, int col_index);
matrix *mat_add(matrix *a, matrix *b);
matrix *mat_minus(matrix *a, matrix *b);
matrix *mat_multiply(matrix *a, matrix *b);
matrix *mat_multiple(matrix *m, double n);
matrix *mat_multiply_strassen(matrix *a, matrix *b);

struct matrix {
    int row;
    int col;
    double *entry;
};

#endif
