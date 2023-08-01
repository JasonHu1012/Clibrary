#include "vector.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define EPSILON 1e-10

void func_test1() {
    int dim = 100;

    printf("vec_zero, vec_dim, vec_kill... ");

    vector *vec = vec_zero(dim);
    assert(vec_dim(vec) == dim);
    for (int i = 0; i < dim; i++) {
        assert(*vec_entry(vec, i) == 0.);
    }
    vec_kill(vec);

    printf("pass\n");
}

void func_test2() {
    int dim = 100;
    int min = -1.;
    int max = 1.;

    printf("vec_rand... ");

    vector *vec = vec_rand(dim, min, max);
    assert(vec_dim(vec) == dim);
    for (int i = 0; i < dim; i++) {
        assert(*vec_entry(vec, i) >= min && *vec_entry(vec, i) <= max);
    }

    vec_kill(vec);

    printf("pass\n");
}

void func_test3() {
    int dim = 100;

    printf("vec_arr... ");

    double *arr = (double *)malloc(sizeof(double) * dim);
    for (int i = 0; i < dim; i++) {
        arr[i] = (double)i;
    }

    vector *vec = vec_arr(dim, arr);
    assert(vec_dim(vec) == dim);
    for (int i = 0; i < dim; i++) {
        assert(*vec_entry(vec, i) == arr[i]);
    }

    vec_kill(vec);

    printf("pass\n");
}

void func_test4() {
    int dim = 100;

    printf("vec_entry... ");

    vector *vec= vec_zero(dim);

    for (int i = 0; i < dim; i++) {
        *vec_entry(vec, i) = (double)i;
    }
    for (int i = 0; i < dim; i++) {
        assert(*vec_entry(vec, i) == (double)i);
    }

    vec_kill(vec);

    printf("pass\n");
}

void func_test5() {
    printf("vec_len... ");

    vector *vec = vec_arr(3, (double[]){ 1.5, 2.5, 3.5 });

    assert(fabs(vec_len(vec) - sqrt(20.75)) < EPSILON);

    vec_kill(vec);

    printf("pass\n");
}

void func_test6() {
    int dim = 100;

    printf("vec_add `dst` can be input arguments... ");

    vector *vec = vec_zero(dim);
    for (int i = 0; i < dim; i++) {
        *vec_entry(vec, i) = (double)i;
    }

    vec_add(vec, vec, vec);
    assert(vec_dim(vec) == dim);
    for (int i = 0; i < dim; i++) {
        assert(fabs(*vec_entry(vec, i) - (double)i * 2.) < EPSILON);
    }

    vec_kill(vec);

    printf("pass\n");
}

void func_test7() {
    int dim = 100;

    printf("vec_minus `dst` can be input arguments... ");

    vector *vec1 = vec_zero(dim);
    for (int i = 0; i < dim; i++) {
        *vec_entry(vec1, i) = (double)i;
    }
    vector *vec2 = vec_zero(dim);
    for (int i = 0; i < dim; i++) {
        *vec_entry(vec2, i) = (double)i * 2.;
    }

    vec_minus(vec1, vec2, vec1);
    assert(vec_dim(vec1) == dim);
    assert(vec_dim(vec2) == dim);
    for (int i = 0; i < dim; i++) {
        assert(*vec_entry(vec1, i) == (double)i * -1.);
    }

    vec_kill(vec1);
    vec_kill(vec2);

    printf("pass\n");
}

void func_test8() {
    printf("vec_dot... ");

    vector *vec1 = vec_arr(3, (double[]){ 1.5, 2.5, 3.5 });
    vector *vec2 = vec_arr(3, (double[]){ 4.5, 5.5, 6.5 });

    assert(fabs(vec_dot(vec1, vec2) - 43.25) < EPSILON);

    vec_kill(vec1);
    vec_kill(vec2);

    printf("pass\n");
}

void func_test9() {
    int dim = 100;
    double mul = 10.;

    printf("vec_mul `dst` can be input arguments... ");

    vector *vec = vec_zero(dim);
    for (int i = 0; i < dim; i++) {
        *vec_entry(vec, i) = (double)i;
    }

    vec_mul(vec, mul, vec);
    assert(vec_dim(vec) == dim);
    for (int i = 0; i < dim; i++) {
        assert(*vec_entry(vec, i) == (double)i * mul);
    }

    vec_kill(vec);

    printf("pass\n");
}

void func_test10() {
    printf("vec_unit `dst` can be input arguments... ");

    vector *vec = vec_arr(3, (double[]){ 3., 4., 12. });

    vec_unit(vec, vec);
    assert(vec_dim(vec) == 3);
    assert(fabs(vec_len(vec) - 1.) < EPSILON);
    assert(fabs(*vec_entry(vec, 0) - 3. / 13.) < EPSILON);
    assert(fabs(*vec_entry(vec, 1) - 4. / 13.) < EPSILON);
    assert(fabs(*vec_entry(vec, 2) - 12. / 13.) < EPSILON);

    vec_kill(vec);

    printf("pass\n");
}

void func_test11() {
    printf("vec_proj `dst` can be input arguments... ");

    vector *vec1 = vec_arr(3, (double[]){ 2., 4., 2. });
    vector *vec2 = vec_arr(3, (double[]){ 0., 3., 4. });

    vec_proj(vec1, vec2, vec1);
    assert(vec_dim(vec1) == 3);
    assert(vec_dim(vec2) == 3);
    assert(fabs(*vec_entry(vec1, 0) - 0.) < EPSILON);
    assert(fabs(*vec_entry(vec1, 1) - 2.4) < EPSILON);
    assert(fabs(*vec_entry(vec1, 2) - 3.2) < EPSILON);
    assert(*vec_entry(vec2, 0) == 0.);
    assert(*vec_entry(vec2, 1) == 3.);
    assert(*vec_entry(vec2, 2) == 4.);

    vec_kill(vec1);
    vec_kill(vec2);

    printf("pass\n");
}

void time_test1() {
    int T = 1000000;
    int dim = 100;
    int min = -1e8;
    int max = 1e8;

    printf("%d vec_add with dimension %d... ", T, dim);

    vector **vec1s = (vector **)malloc(sizeof(vector *) * T);
    for (int i = 0; i < T; i++) {
        vec1s[i] = vec_rand(dim, min, max);
    }
    vector **vec2s = (vector **)malloc(sizeof(vector *) * T);
    for (int i = 0; i < T; i++) {
        vec2s[i] = vec_rand(dim, min, max);
    }
    vector **dsts = (vector **)malloc(sizeof(vector *) * T);
    for (int i = 0; i < T; i++) {
        dsts[i] = vec_rand(dim, min, max);
    }

    clock_t start = clock();

    for (int i = 0; i < T; i++) {
        vec_add(vec1s[i], vec2s[i], dsts[i]);
    }

    clock_t end = clock();
    for (int i = 0; i < T; i++) {
        vec_kill(vec1s[i]);
        vec_kill(vec2s[i]);
        vec_kill(dsts[i]);
    }
    free(vec1s);
    free(vec2s);
    free(dsts);

    printf("%f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

void time_test2() {
    int T = 1000000;
    int dim = 100;
    int min = -1e8;
    int max = 1e8;

    printf("%d vec_dot with dimension %d... ", T, dim);

    vector **vec1s = (vector **)malloc(sizeof(vector *) * T);
    for (int i = 0; i < T; i++) {
        vec1s[i] = vec_rand(dim, min, max);
    }
    vector **vec2s = (vector **)malloc(sizeof(vector *) * T);
    for (int i = 0; i < T; i++) {
        vec2s[i] = vec_rand(dim, min, max);
    }

    clock_t start = clock();

    for (int i = 0; i < T; i++) {
        vec_dot(vec1s[i], vec2s[i]);
    }

    clock_t end = clock();
    for (int i = 0; i < T; i++) {
        vec_kill(vec1s[i]);
        vec_kill(vec2s[i]);
    }
    free(vec1s);
    free(vec2s);

    printf("%f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

int main() {
    printf("=== start vector tests ===\n");

    func_test1();
    func_test2();
    func_test3();
    func_test4();
    func_test5();
    func_test6();
    func_test7();
    func_test8();
    func_test9();
    func_test10();
    func_test11();

    time_test1();
    time_test2();

    printf("=== pass vector tests ===\n");

    return 0;
}
