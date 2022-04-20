#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

static matrix *new_mat(int row_size, int col_size) {
    matrix *output = (matrix *)malloc(sizeof(matrix));
    output->row = row_size;
    output->col = col_size;
    output->entry = (double *)malloc(sizeof(double) * row_size * col_size);
    return output;
}

matrix *mat_random(int row_size, int col_size, double min, double max, bool integer) {
    assert(row_size > 0 && col_size > 0);
    assert(min <= max);
    static char init = 0;
    matrix *output = new_mat(row_size, col_size);
    if (!init) {
        srand((unsigned int)time(NULL));
        init = 1;
    }
    int i, s = row_size * col_size;
    for (i = 0; i < s; i++) {
        output->entry[i] = integer ? rand() % ((int)max - (int)min + 1) + (int)min : (max - min) * rand() / RAND_MAX + min;
    }
    return output;
}

matrix *mat_zero(int row_size, int col_size) {
    assert(row_size > 0 && col_size > 0);
    matrix *output = (matrix *)malloc(sizeof(matrix));
    output->row = row_size;
    output->col = col_size;
    output->entry = (double *)calloc(row_size * col_size, sizeof(double));
    return output;
}

matrix *mat_one(int row_size, int col_size) {
    assert(row_size > 0 && col_size > 0);
    matrix *output = new_mat(row_size, col_size);
    int i, s = row_size * col_size;
    for (i = 0; i < s; i++) {
        output->entry[i] = 1;
    }
    return output;
}

matrix *mat_identity(int size) {
    assert(size > 0);
    matrix *output = mat_zero(size, size);
    int i;
    for (i = 0; i < size; i++) {
        output->entry[i * size + i] = 1;
    }
    return output;
}

matrix *mat_transpose(matrix *m) {
    matrix *output = new_mat(m->col, m->row);
    int i, j;
    for (i = 0; i < m->col; i++) {
        for (j = 0; j < m->row; j++) {
            output->entry[i * m->row + j] = m->entry[j * m->col + i];
        }
    }
    return output;
}

matrix *mat_read(int row_size, int col_size) {
    assert(row_size > 0 && col_size > 0);
    matrix *output = new_mat(row_size, col_size);
    int i, s = row_size * col_size;
    for (i = 0; i < s; i++) {
        scanf("%lf", &output->entry[i]);
    }
    return output;
}

void mat_print(matrix *m) {
    int i, j;
    for (i = 0; i < m->row; i++) {
        printf("%.2e", m->entry[i * m->col]);
        for (j = 1; j < m->col; j++) {
            printf(" %.2e", m->entry[i * m->col + j]);
        }
        printf("\n");
    }
}

void mat_kill(matrix *m) {
    free(m->entry);
    free(m);
}

double *mat_entry(matrix *m, int row_index, int col_index) {
    assert(row_index >= 1 && row_index <= m->row);
    assert(col_index >= 1 && col_index <= m->col);
    return &m->entry[(row_index - 1) * m->col + col_index - 1];
}

matrix *mat_add(matrix *a, matrix *b) {
    assert(a->row == b->row && a->col == b->col);
    matrix *output = new_mat(a->row, a->col);
    int i, s = output->row * output->col;
    for (i = 0; i < s; i++) {
        output->entry[i] = a->entry[i] + b->entry[i];
    }
    return output;
}

matrix *mat_minus(matrix *a, matrix *b) {
    assert(a->row == b->row && a->col == b->col);
    matrix *output = new_mat(a->row, a->col);
    int i, s = output->row * output->col;
    for (i = 0; i < s; i++) {
        output->entry[i] = a->entry[i] - b->entry[i];
    }
    return output;
}

matrix *mat_multiply(matrix *a, matrix *b) {
    assert(a->col == b->row);
    matrix *output = mat_zero(a->row, b->col);
    matrix *b_ = mat_transpose(b);
    int i, j, k;
    for (i = 0; i < a->row; i++) {
        for (j = 0; j < b->col; j++) {
            for (k = 0; k < a->col; k++) {
                output->entry[i * b->col + j] += a->entry[i * a->col + k] * b_->entry[j * b->row + k];
            }
        }
    }
    mat_kill(b_);
    return output;
}

static matrix *add(matrix *a, matrix *b, int ai, int aj, int bi, int bj, int rs, int cs) {
    matrix *output = new_mat(rs, cs);
    int i, j;
    for (i = 0; i < rs; i++) {
        for (j = 0; j < cs; j++) {
            output->entry[i * cs + j] = a->entry[(ai + i) * a->col + aj + j] + b->entry[(bi + i) * b->col + bj + j];
        }
    }
    return output;
}

static matrix *minus(matrix *a, matrix *b, int ai, int aj, int bi, int bj, int rs, int cs) {
    matrix *output = new_mat(rs, cs);
    int i, j;
    for (i = 0; i < rs; i++) {
        for (j = 0; j < cs; j++) {
            output->entry[i * cs + j] = a->entry[(ai + i) * a->col + aj + j] - b->entry[(bi + i) * b->col + bj + j];
        }
    }
    return output;
}

static matrix *mul(matrix *a, matrix *b, int ai, int aj, int bi, int bj, int ars, int common, int bcs) {
    matrix *output = new_mat(ars, bcs);
    int i, j, k;
    if (ars == 1 || common == 1 || bcs == 1) {
        for (i = 0; i < ars; i++) {
            for (j = 0; j < bcs; j++) {
                output->entry[i * bcs + j] = 0.0;
                for (k = 0; k < common; k++) {
                    output->entry[i * bcs + j] += a->entry[(ai + i) * a->col + aj + k] * b->entry[(bi + k) * b->col + bj + j];
                }
            }
        }
    }
    else {
        int ars_half = ars >> 1, common_half = common >> 1, bcs_half = bcs >> 1;
        int common_middle = common & 1;
        int ars_i = ars_half + (ars & 1), common_i = common_half + common_middle, bcs_i = bcs_half + (bcs & 1);
        matrix **s = (matrix **)malloc(sizeof(matrix *) * 10);
        s[0] = minus(b, b, bi, bj + bcs_i, bi + common_i, bj + bcs_i, common_half, bcs_half);
        s[1] = add(a, a, ai, aj, ai, aj + common_i, ars_half, common_half);
        s[2] = add(a, a, ai + ars_i, aj, ai + ars_i, aj + common_i, ars_half, common_half);
        s[3] = minus(b, b, bi + common_i, bj, bi, bj, common_half, bcs_half);
        s[4] = add(a, a, ai, aj, ai + ars_i, aj + common_i, ars_half, common_half);
        s[5] = add(b, b, bi, bj, bi + common_i, bj + bcs_i, common_half, bcs_half);
        s[6] = minus(a, a, ai, aj + common_i, ai + ars_i, aj + common_i, ars_half, common_half);
        s[7] = add(b, b, bi + common_i, bj, bi + common_i, bj + bcs_i, common_half, bcs_half);
        s[8] = minus(a, a, ai, aj, ai + ars_i, aj, ars_half, common_half);
        s[9] = add(b, b, bi, bj, bi, bj + bcs_i, common_half, bcs_half);
        matrix **p = (matrix **)malloc(sizeof(matrix *) * 7);
        p[0] = mul(a, s[0], ai, aj, 0, 0, ars_half, common_half, bcs_half);
        p[1] = mul(s[1], b, 0, 0, bi + common_i, bj + bcs_i, ars_half, common_half, bcs_half);
        p[2] = mul(s[2], b, 0, 0, bi, bj, ars_half, common_half, bcs_half);
        p[3] = mul(a, s[3], ai + ars_i, aj + common_i, 0, 0, ars_half, common_half, bcs_half);
        p[4] = mul(s[4], s[5], 0, 0, 0, 0, ars_half, common_half, bcs_half);
        p[5] = mul(s[6], s[7], 0, 0, 0, 0, ars_half, common_half, bcs_half);
        p[6] = mul(s[8], s[9], 0, 0, 0, 0, ars_half, common_half, bcs_half);
        for (i = 0; i < ars_half; i++) {
            for (j = 0; j < bcs_half; j++) {
                output->entry[i * bcs + j] = p[4]->entry[i * bcs_half + j] + p[3]->entry[i * bcs_half + j] - p[1]->entry[i * bcs_half + j] + p[5]->entry[i * bcs_half + j] + (common_middle ? a->entry[(ai + i) * a->col + aj + common_half] * b->entry[(bi + common_half) * b->col + bj + j] : 0.0);
            }
            for (; j < bcs_i; j++) {
                output->entry[i * bcs + j] = 0.0;
                for (k = 0; k < common; k++) {
                    output->entry[i * bcs + j] += a->entry[(ai + i) * a->col + aj + k] * b->entry[(bi + k) * b->col + bj + j];
                }
            }
            for (; j < bcs; j++) {
                output->entry[i * bcs + j] = p[0]->entry[i * bcs_half + j - bcs_i] + p[1]->entry[i * bcs_half + j - bcs_i] + (common_middle ? a->entry[(ai + i) * a->col + aj + common_half] * b->entry[(bi + common_half) * b->col + bj + j] : 0.0);
            }
        }
        for (; i < ars_i; i++) {
            for (j = 0; j < bcs; j++) {
                output->entry[i * bcs + j] = 0.0;
                for (k = 0; k < common; k++) {
                    output->entry[i * bcs + j] += a->entry[(ai + i) * a->col + aj + k] * b->entry[(bi + k) * b->col + bj + j];
                }
            }
        }
        for (; i < ars; i++) {
            for (j = 0; j < bcs_half; j++) {
                output->entry[i * bcs + j] = p[2]->entry[(i - ars_i) * bcs_half + j] + p[3]->entry[(i - ars_i) * bcs_half + j] + (common_middle ? a->entry[(ai + i) * a->col + aj + common_half] * b->entry[(bi + common_half) * b->col + bj + j] : 0.0);
            }
            for (; j < bcs_i; j++) {
                output->entry[i * bcs + j] = 0.0;
                for (k = 0; k < common; k++) {
                    output->entry[i * bcs + j] += a->entry[(ai + i) * a->col + aj + k] * b->entry[(bi + k) * b->col + bj + j];
                }
            }
            for (; j < bcs; j++) {
                output->entry[i * bcs + j] = p[4]->entry[(i - ars_i) * bcs_half + j - bcs_i] + p[0]->entry[(i - ars_i) * bcs_half + j - bcs_i] - p[2]->entry[(i - ars_i) * bcs_half + j - bcs_i] - p[6]->entry[(i - ars_i) * bcs_half + j - bcs_i] + (common_middle ? a->entry[(ai + i) * a->col + aj + common_half] * b->entry[(bi + common_half) * b->col + bj + j] : 0.0);
            }
        }
        for (i = 0; i < 10; i++) {
            mat_kill(s[i]);
        }
        for (i = 0; i < 7; i++) {
            mat_kill(p[i]);
        }
    }
    return output;
}

matrix *mat_multiple(matrix *m, double n) {
    matrix *output = new_mat(m->row, m->col);
    int i, s = output->row * output->col;
    for (i = 0; i < s; i++) {
        output->entry[i] = m->entry[i] * n;
    }
    return output;
}

matrix *mat_multiply_strassen(matrix *a, matrix *b) {
    assert(a->col == b->row);
    return mul(a, b, 0, 0, 0, 0, a->row, a->col, b->col);
}

