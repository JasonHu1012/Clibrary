#include "stack.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void func_test1() {
    printf("stk_init, stk_kill... ");

    stack *stk = stk_init(sizeof(int));
    assert(stk_size(stk) == 0);
    stk_kill(stk);

    printf("pass\n");
}

void func_test2() {
    int size = 100;
    printf("stk_size, stk_is_empty... ");

    stack *stk = stk_init(sizeof(int));
    for (int i = 0; i < size; i++) {
        stk_push(stk, &i);
    }

    assert(stk_size(stk) == size);
    assert(!stk_is_empty(stk));

    for (int i = 0; i < size; i++) {
        stk_pop(stk, NULL);
    }

    assert(stk_size(stk) == 0);
    assert(stk_is_empty(stk));

    stk_kill(stk);

    printf("pass\n");
}

void func_test3() {
    int T = 1000;
    printf("stk_push, stk_pop... ");

    stack *stk = stk_init(sizeof(int));

    for (int i = 0; i < T; i++) {
        int n = i + T;
        stk_push(stk, &n);
        assert(n == i + T);
        assert(stk_size(stk) == i + 1);
    }
    for (int i = T - 1; i > -1; i--) {
        int n = -1;
        stk_pop(stk, &n);
        assert(n == i + T);
        assert(stk_size(stk) == i);
    }

    stk_kill(stk);

    printf("pass\n");
}

void func_test4() {
    int value = 100;
    printf("stk_top... ");

    stack *stk = stk_init(sizeof(int));
    int n = value;
    stk_push(stk, &n);
    n--;

    stk_top(stk, &n);
    assert(n == value);
    assert(stk_size(stk) == 1);

    stk_kill(stk);

    printf("pass\n");
}

void func_test5() {
    printf("stk_pop `dst` can be NULL... ");

    stack *stk = stk_init(sizeof(int));
    int n;
    stk_push(stk, &n);

    stk_pop(stk, NULL);
    assert(stk_size(stk) == 0);

    stk_kill(stk);

    printf("pass\n");
}

void time_test1() {
    int T = 10000000;

    printf("%d stk_push and stk_pop... ", T);

    stack *stk = stk_init(sizeof(int));

    int *values = (int *)malloc(sizeof(int) * T);
    for (int i = 0; i < T; i++) {
        values[i] = rand();
    }

    clock_t start = clock();

    for (int i = 0; i < T; i++) {
        stk_push(stk, &values[i]);
    }
    for (int i = 0; i < T; i++) {
        stk_pop(stk, &values[i]);
    }

    clock_t end = clock();
    stk_kill(stk);
    free(values);

    printf("%f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

int main() {
    printf("=== start stack tests ===\n");

    func_test1();
    func_test2();
    func_test3();
    func_test4();
    func_test5();

    time_test1();

    printf("=== pass stack tests ===\n");

    return 0;
}
