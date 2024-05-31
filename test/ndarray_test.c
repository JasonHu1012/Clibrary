#include "ndarray.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void func_test1() {
    int sizes[3] = { 50, 100, 20 };

    printf("nda_init, nda_kill... ");

    ndarray *nda = nda_init(sizes, sizeof(sizes) / sizeof(int), sizeof(int));
    nda_kill(nda);

    printf("pass\n");
}

void func_test2() {
    printf("nda_get, nda_set... ");

    int sizes[3] = { 50, 100, 20 };
    ndarray *nda = nda_init(sizes, sizeof(sizes) / sizeof(int), sizeof(int));

    for (int i = 0; i < sizes[0]; i++) {
        for (int j = 0; j < sizes[1]; j++) {
            for (int k = 0; k < sizes[2]; k++) {
                int n = (i + 1) * (j + 1) + k;
                nda_set(nda, (int[]){ i, j, k }, &n);
                assert(n == (i + 1) * (j + 1) + k);
            }
        }
    }
    for (int i = 0; i < sizes[0]; i++) {
        for (int j = 0; j < sizes[1]; j++) {
            for (int k = 0; k < sizes[2]; k++) {
                int n = -1;
                nda_get(nda, (int[]){ i, j, k }, &n);
                assert(n == (i + 1) * (j + 1) + k);
            }
        }
    }

    nda_kill(nda);

    printf("pass\n");
}

void time_test1() {
    int T = 10000000;

    printf("%d nda_get and nda_set... ", T);

    int sizes[3] = { 50, 100, 20 };
    ndarray *nda = nda_init(sizes, sizeof(sizes) / sizeof(int), sizeof(int));

    int *values = (int *)malloc(sizeof(int) * T);
    for (int i = 0; i < T; i++) {
        values[i] = rand();
    }
    int **indexes = (int **)malloc(sizeof(int *) * T);
    for (int i = 0; i < T; i++) {
        indexes[i] = (int *)malloc(sizeof(int) * sizeof(sizes) / sizeof(int));
        for (int j = 0; j < sizeof(sizes) / sizeof(int); j++) {
            indexes[i][j] = rand() % sizes[j];
        }
    }

    clock_t start = clock();

    for (int i = 0; i < T; i++) {
        nda_set(nda, indexes[i], &values[i]);
    }
    for (int i = 0; i < T; i++) {
        nda_get(nda, indexes[i], &values[i]);
    }

    clock_t end = clock();
    nda_kill(nda);
    free(values);
    for (int i = 0; i < T; i++) {
        free(indexes[i]);
    }
    free(indexes);

    printf("%f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("=== start ndarray tests ===\n");

    func_test1();
    func_test2();

    time_test1();

    printf("=== pass ndarray tests ===\n");

    return 0;
}
