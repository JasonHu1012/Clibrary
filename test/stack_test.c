#include "stack.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void test1() {
    printf("stk_init, stk_kill... ");

    stack *stk = stk_init(sizeof(int));
    assert(stk_size(stk) == 0);
    stk_kill(stk);

    printf("pass\n");
}

void test2() {
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

void test3() {
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

void test4() {
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

void test5() {
    printf("stk_pop `dst` can be NULL... ");

    stack *stk = stk_init(sizeof(int));
    int n;
    stk_push(stk, &n);

    stk_pop(stk, NULL);
    assert(stk_size(stk) == 0);

    stk_kill(stk);

    printf("pass\n");
}

void test6() {
    int T = 10000000;

    printf("%d stk_push and stk_pop... ", T);

    stack *stk = stk_init(sizeof(int));
    int n;
    clock_t start = clock();

    for (int i = 0; i < T; i++) {
        n = rand();
        stk_push(stk, &n);
    }
    for (int i = 0; i < T; i++) {
        stk_pop(stk, &n);
    }

    clock_t end = clock();
    stk_kill(stk);

    printf("%f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();

    return 0;
}
