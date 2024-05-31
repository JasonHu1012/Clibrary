#include "llist.h"
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

void func_test1() {
    printf("llst_init, llst_kill... ");

    llist *llst = llst_init(sizeof(int));
    assert(llst_size(llst) == 0);
    llst_kill(llst);

    printf("pass\n");
}

void func_test2() {
    printf("llst_it_init, llst_it_kill... ");

    llist *llst = llst_init(sizeof(int));
    int n;
    llst_append(llst, &n, LLST_TAIL);

    llist_iter *llst_it = llst_it_init(llst, LLST_HEAD);
    llst_it_kill(llst_it);

    llst_kill(llst);

    printf("pass\n");
}

void func_test3() {
    int T = 1000;

    printf("llst_append, llst_remove... ");

    llist *llst = llst_init(sizeof(int));
    int n;

    for (int i = 0; i < T; i++) {
        n = i * 2;
        llst_append(llst, &n, LLST_TAIL);
        assert(llst_size(llst) == i + 1);
        assert(n == i * 2);
    }
    for (int i = 0; i < T; i++) {
        n = -1;
        llst_remove(llst, &n, LLST_HEAD);
        assert(llst_size(llst) == T - i - 1);
        assert(n == i * 2);
    }

    llst_kill(llst);

    printf("pass\n");
}

void func_test4() {
    int size = 1000;

    printf("llst_size, llst_is_empty... ");

    llist *llst = llst_init(sizeof(int));
    for (int i = 0; i < size; i++) {
        llst_append(llst, &i, LLST_HEAD);
    }

    assert(llst_size(llst) == size);
    assert(!llst_is_empty(llst));

    for (int i = 0; i < size; i++) {
        llst_remove(llst, NULL, LLST_HEAD);
    }

    assert(llst_size(llst) == 0);
    assert(llst_is_empty(llst));

    llst_kill(llst);

    printf("pass\n");
}

void func_test5() {
    int size = 1000;

    printf("llst_it_move... ");

    llist *llst = llst_init(sizeof(int));
    for (int i = 0; i < size; i++) {
        llst_append(llst, &i, LLST_HEAD);
    }
    llist_iter *llst_it = llst_it_init(llst, LLST_HEAD);

    for (int i = 0; i < size - 1; i++) {
        assert(llst_it_move(llst_it, LLST_TAIL));
    }
    assert(!llst_it_move(llst_it, LLST_TAIL));
    for (int i = 0; i < size - 1; i++) {
        assert(llst_it_move(llst_it, LLST_HEAD));
    }
    assert(!llst_it_move(llst_it, LLST_HEAD));

    llst_it_kill(llst_it);
    llst_kill(llst);

    printf("pass\n");
}

void func_test6() {
    int size = 1000;

    printf("llst_it_move_to... ");

    llist *llst = llst_init(sizeof(int));
    for (int i = 0; i < size; i++) {
        llst_append(llst, &i, LLST_HEAD);
    }
    llist_iter *llst_it = llst_it_init(llst, LLST_HEAD);

    for (int i = 0; i < size - 1; i++) {
        assert(llst_it_move(llst_it, LLST_TAIL));
    }
    assert(!llst_it_move(llst_it, LLST_TAIL));
    llst_it_move_to(llst_it, LLST_HEAD);
    for (int i = 0; i < size - 1; i++) {
        assert(llst_it_move(llst_it, LLST_TAIL));
    }
    assert(!llst_it_move(llst_it, LLST_TAIL));

    llst_it_kill(llst_it);
    llst_kill(llst);

    printf("pass\n");
}

void func_test7() {
    int size = 1000;

    printf("llst_it_get, llst_it_set... ");

    llist *llst = llst_init(sizeof(int)); 
    for (int i = 0; i < size; i++) {
        llst_append(llst, &i, LLST_HEAD);
    }
    llist_iter *llst_it = llst_it_init(llst, LLST_HEAD);
    int n;

    for (int i = 0; i < size; i++) {
        n = i * 2;
        llst_it_set(llst_it, &n);
        llst_it_move(llst_it, LLST_TAIL);
        assert(n == i * 2);
    }
    llst_it_move_to(llst_it, LLST_HEAD);
    for (int i = 0; i < size; i++) {
        n = -1;
        llst_it_get(llst_it, &n);
        llst_it_move(llst_it, LLST_TAIL);
        assert(n == i * 2);
    }

    llst_it_kill(llst_it);
    llst_kill(llst);

    printf("pass\n");
}

void func_test8() {
    int T = 1000;

    printf("llst_it_insert, llst_it_remove... ");

    llist *llst = llst_init(sizeof(int)); 
    int n;
    llst_append(llst, &n, LLST_TAIL);
    llist_iter *llst_it = llst_it_init(llst, LLST_HEAD);

    for (int i = 0; i < T; i++) {
        n = i * 2;
        llst_it_insert(llst_it, &n , LLST_TAIL);
        assert(n == i * 2);
        assert(llst_size(llst) == i + 2);
    }
    llst_it_move_to(llst_it, LLST_TAIL);
    llst_it_move(llst_it, LLST_HEAD);
    for (int i = 1; i < T; i++) {
        n = -1;
        llst_it_remove(llst_it, &n, LLST_HEAD);
        assert(n == i * 2);
        assert(llst_size(llst) == T + 1 - i);
    }

    llst_it_kill(llst_it);
    llst_kill(llst);

    printf("pass\n");
}

void func_test9() {
    int size = 1000;

    printf("llst_remove `dst` can be NULL... ");

    llist *llst = llst_init(sizeof(int)); 
    for (int i = 0; i < size; i++) {
        llst_append(llst, &i, LLST_HEAD);
    }

    for (int i = 0; i < size; i++) {
        llst_remove(llst, NULL, LLST_HEAD);
        assert(llst_size(llst) == size - 1 - i);
    }

    llst_kill(llst);

    printf("pass\n");
}

void func_test10() {
    int size = 1000;

    printf("llst_it_remove `dst` can be NULL... ");

    llist *llst = llst_init(sizeof(int)); 
    for (int i = 0; i < size; i++) {
        llst_append(llst, &i, LLST_HEAD);
    }
    llist_iter *llst_it = llst_it_init(llst, LLST_HEAD);

    for (int i = 0; i < size; i++) {
        llst_it_remove(llst_it, NULL, LLST_HEAD);
        assert(llst_size(llst) == size - 1 - i);
    }

    llst_it_kill(llst_it);
    llst_kill(llst);

    printf("pass\n");
}

void time_test1() {
    int size = 10000000;

    printf("%d llst_append and llst_remove... ", size);

    llist *llst = llst_init(sizeof(int));

    int *values = (int *)malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++) {
        values[i] = rand();
    }

    clock_t start = clock();

    for (int i = 0; i < size; i++) {
        llst_append(llst, &values[i], LLST_TAIL);
    }
    for (int i = 0; i < size; i++) {
        llst_remove(llst, &values[i], LLST_HEAD);
    }

    clock_t end = clock();
    llst_kill(llst);
    free(values);

    printf("%f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

void time_test2() {
    int size = 10000000;

    printf("%d llst_it_move and llst_it_get... ", size);

    llist *llst = llst_init(sizeof(int));
    int n;
    for (int i = 0; i < size; i++) {
        n = rand();
        llst_append(llst, &n, LLST_TAIL);
    }
    llist_iter *llst_it = llst_it_init(llst, LLST_HEAD);
    clock_t start = clock();

    for (int i = 0; i < size; i++) {
        llst_it_get(llst_it, &n);
        llst_it_move(llst_it, LLST_TAIL);
    }

    clock_t end = clock();
    llst_it_kill(llst_it);
    llst_kill(llst);

    printf("%f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("=== start llist tests ===\n");

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

    printf("=== pass llist tests ===\n");

    return 0;
}
