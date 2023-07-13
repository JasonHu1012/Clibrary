#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void func_test1() {
    printf("lst_init, lst_kill... ");

    list *lst = lst_init(sizeof(int));
    assert(lst_size(lst) == 0);
    lst_kill(lst);

    printf("pass\n");
}

void func_test2() {
    int size = 1000;

    printf("lst_set_size, lst_size... ");

    list *lst = lst_init(sizeof(int));

    lst_set_size(lst, size);
    assert(lst_size(lst) == size);

    lst_kill(lst);

    printf("pass\n");
}

void func_test3() {
    printf("lst_is_empty... ");

    list *lst = lst_init(sizeof(int));

    lst_set_size(lst, 0);
    assert(lst_is_empty(lst));

    lst_kill(lst);

    printf("pass\n");
}

void func_test4() {
    int size = 1000;

    printf("lst_get, lst_set... ");

    list *lst = lst_init(sizeof(int));
    lst_set_size(lst, size);

    for (int i = 0; i < size; i++) {
        int n = i + size;
        lst_set(lst, i, &n);
        assert(n == i + size);
    }
    for (int i = size - 1; i > -1; i--) {
        int n = -1;
        lst_get(lst, i, &n);
        assert(n == i + size);
    }

    lst_kill(lst);

    printf("pass\n");
}

void func_test5() {
    int size = 1000;

    printf("lst_append, lst_pop... ");

    list *lst = lst_init(sizeof(int));

    for (int i = 0; i < size; i++) {
        int n = i + size;
        lst_append(lst, &n);
        assert(n == i + size);
        assert(lst_size(lst) == i + 1);
    }
    for (int i = size - 1; i > -1; i--) {
        int n = -1;
        lst_pop(lst, &n);
        assert(n == i + size);
        assert(lst_size(lst) == i);
    }

    lst_kill(lst);

    printf("pass\n");
}

void func_test6() {
    printf("lst_pop `dst` can be NULL... ");

    list *lst = lst_init(sizeof(int));
    int n;
    lst_append(lst, &n);

    lst_pop(lst, NULL);

    lst_kill(lst);

    printf("pass\n");
}

void func_test7() {
    int size = 100;
    int T = 1000;
    int index = 50;

    printf("lst_insert, lst_remove... ");

    list *lst = lst_init(sizeof(int));
    lst_set_size(lst, size);

    for (int i = 0; i < T; i++) {
        int n = i + size;
        lst_insert(lst, index, &n);
        assert(n == i + size);
        assert(lst_size(lst) == size + i + 1);
    }
    for (int i = T - 1; i > -1; i--) {
        int n = -1;
        lst_remove(lst, index, &n);
        assert(n == i + size);
        assert(lst_size(lst) == size + i);
    }

    printf("pass\n");
}

void func_test8() {
    int size = 100;
    int index = 50;

    printf("lst_remove `dst` can be NULL... ");

    list *lst = lst_init(sizeof(int));
    lst_set_size(lst, size);

    lst_remove(lst, index, NULL);

    lst_kill(lst);

    printf("pass\n");
}

void time_test1() {
    int size = 10000;
    int T = 10000000;

    printf("%d lst_get and lst_set... ", T);

    list *lst = lst_init(sizeof(int));
    lst_set_size(lst, size);
    int n;
    clock_t start = clock();

    for (int i = 0; i < T; i++) {
        n = rand();
        lst_set(lst, rand() % size, &n);
    }
    for (int i = 0; i < T; i++) {
        lst_get(lst, rand() % size, &n);
    }

    clock_t end = clock();
    lst_kill(lst);

    printf("%f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

int main() {
    printf("=== start list tests ===\n");

    func_test1(); 
    func_test2();
    func_test3();
    func_test4();
    func_test5();
    func_test6();
    func_test7();
    func_test8();

    time_test1();

    printf("=== pass list tests ===\n");

    return 0;
}
