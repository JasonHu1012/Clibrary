#include "deque.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void func_test1() {
    printf("deq_init, deq_kill... ");

    deque *deq = deq_init(sizeof(int));
    assert(deq_size(deq) == 0);
    deq_kill(deq);

    printf("pass\n");
}

void func_test2() {
    int size = 100;

    printf("deq_size, deq_is_empty... ");

    deque *deq = deq_init(sizeof(int));
    for (int i = 0; i < size; i++) {
        deq_push_tail(deq, &i);
    }

    assert(deq_size(deq) == size);
    assert(!deq_is_empty(deq));

    for (int i = 0; i < size; i++) {
        deq_pop_head(deq, NULL);
    }

    assert(deq_size(deq) == 0);
    assert(deq_is_empty(deq));

    deq_kill(deq);

    printf("pass\n");
}

void func_test3() {
    int T = 10000;

    printf("deq_push_head, deq_pop_tail... ");

    deque *deq = deq_init(sizeof(int));

    for (int i = 0; i < T; i++) {
        int n = i + T;
        deq_push_head(deq, &n);
        assert(n == i + T);
        assert(deq_size(deq) == i + 1);
    }
    for (int i = 0; i < T; i++) {
        int n = -1;
        deq_pop_tail(deq, &n);
        assert(n == i + T);
        assert(deq_size(deq) == T - i - 1);
    }

    deq_kill(deq);

    printf("pass\n");
}

void func_test4() {
    int T = 10000;

    printf("deq_push_tail, deq_pop_head... ");

    deque *deq = deq_init(sizeof(int));

    for (int i = 0; i < T; i++) {
        int n = i + T;
        deq_push_tail(deq, &n);
        assert(n == i + T);
        assert(deq_size(deq) == i + 1);
    }
    for (int i = 0; i < T; i++) {
        int n = -1;
        deq_pop_head(deq, &n);
        assert(n == i + T);
        assert(deq_size(deq) == T - i - 1);
    }

    deq_kill(deq);

    printf("pass\n");
}

void func_test5() {
    int T = 10000;

    printf("deq_push_head, deq_pop_head... ");

    deque *deq = deq_init(sizeof(int));

    for (int i = 0; i < T; i++) {
        int n = i + T;
        deq_push_head(deq, &n);
        assert(n == i + T);
        assert(deq_size(deq) == i + 1);
    }
    for (int i = T - 1; i > -1; i--) {
        int n = -1;
        deq_pop_head(deq, &n);
        assert(n == i + T);
        assert(deq_size(deq) == i);
    }

    deq_kill(deq);

    printf("pass\n");
}

void func_test6() {
    int T = 10000;

    printf("deq_push_tail, deq_pop_tail... ");

    deque *deq = deq_init(sizeof(int));

    for (int i = 0; i < T; i++) {
        int n = i + T;
        deq_push_tail(deq, &n);
        assert(n == i + T);
        assert(deq_size(deq) == i + 1);
    }
    for (int i = T - 1; i > -1; i--) {
        int n = -1;
        deq_pop_tail(deq, &n);
        assert(n == i + T);
        assert(deq_size(deq) == i);
    }

    deq_kill(deq);

    printf("pass\n");
}

void func_test7() {
    int value = 100;

    printf("deq_head... ");

    deque *deq = deq_init(sizeof(int));
    deq_push_head(deq, &value);
    int n = value - 1;

    deq_head(deq, &n);
    assert(n == value);
    assert(deq_size(deq) == 1);

    deq_kill(deq);

    printf("pass\n");
}

void func_test8() {
    int value = 100;

    printf("deq_tail... ");

    deque *deq = deq_init(sizeof(int));
    deq_push_tail(deq, &value);
    int n = value - 1;

    deq_tail(deq, &n);
    assert(n == value);
    assert(deq_size(deq) == 1);

    deq_kill(deq);

    printf("pass\n");
}

void func_test9() {
    printf("deq_pop_head `dst` can be NULL... ");

    deque *deq = deq_init(sizeof(int));
    int n;
    deq_push_head(deq, &n);

    deq_pop_head(deq, NULL);
    assert(deq_size(deq) == 0);

    deq_kill(deq);

    printf("pass\n");
}

void func_test10() {
    printf("deq_pop_tail `dst` can be NULL... ");

    deque *deq = deq_init(sizeof(int));
    int n;
    deq_push_tail(deq, &n);

    deq_pop_tail(deq, NULL);
    assert(deq_size(deq) == 0);

    deq_kill(deq);

    printf("pass\n");
}

void time_test1() {
    int T = 10000000;

    printf("%d deq_push_head and deq_pop_head... ", T);

    deque *deq = deq_init(sizeof(int));

    int *values = (int *)malloc(sizeof(int) * T);
    for (int i = 0; i < T; i++) {
        values[i] = rand();
    }

    clock_t start = clock();

    for (int i = 0; i < T; i++) {
        deq_push_head(deq, &values[i]);
    }
    for (int i = 0; i < T; i++) {
        deq_pop_head(deq, &values[i]);
    }

    clock_t end = clock();
    deq_kill(deq);
    free(values);

    printf("%f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

void time_test2() {
    int T = 10000000;

    printf("%d deq_push_tail and deq_pop_tail... ", T);

    deque *deq = deq_init(sizeof(int));

    int *values = (int *)malloc(sizeof(int) * T);
    for (int i = 0; i < T; i++) {
        values[i] = rand();
    }

    clock_t start = clock();

    for (int i = 0; i < T; i++) {
        deq_push_tail(deq, &values[i]);
    }
    for (int i = 0; i < T; i++) {
        deq_pop_tail(deq, &values[i]);
    }

    clock_t end = clock();
    deq_kill(deq);
    free(values);

    printf("%f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

void time_test3() {
    int T = 10000000;

    printf("%d deq_push_tail and deq_pop_head... ", T);

    deque *deq = deq_init(sizeof(int));

    int *values = (int *)malloc(sizeof(int) * T);
    for (int i = 0; i < T; i++) {
        values[i] = rand();
    }

    clock_t start = clock();

    for (int i = 0; i < T; i++) {
        deq_push_tail(deq, &values[i]);
    }
    for (int i = 0; i < T; i++) {
        deq_pop_head(deq, &values[i]);
    }

    clock_t end = clock();
    deq_kill(deq);
    free(values);

    printf("%f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

void time_test4() {
    int T = 10000000;

    printf("%d deq_push_head and deq_pop_tail... ", T);

    deque *deq = deq_init(sizeof(int));

    int *values = (int *)malloc(sizeof(int) * T);
    for (int i = 0; i < T; i++) {
        values[i] = rand();
    }

    clock_t start = clock();

    for (int i = 0; i < T; i++) {
        deq_push_head(deq, &values[i]);
    }
    for (int i = 0; i < T; i++) {
        deq_pop_tail(deq, &values[i]);
    }

    clock_t end = clock();
    deq_kill(deq);
    free(values);

    printf("%f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("=== start deque tests ===\n");

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

    time_test1();
    time_test2();
    time_test3();
    time_test4();

    printf("=== pass deque tests ===\n");

    return 0;
}
