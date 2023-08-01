#include "table.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void func_test1() {
    printf("tbl_init, tbl_kill... ");

    table *tbl = tbl_init(sizeof(int));
    assert(tbl_size(tbl) == 0);
    tbl_kill(tbl);

    printf("pass\n");
}

void func_test2() {
    int size = 1000;

    printf("tbl_get, tbl_set... ");

    table *tbl = tbl_init(sizeof(int));
    char key[16];
    char key_backup[16];
    int value;

    for (int i = 0; i < size; i++) {
        sprintf(key, "key%d", i);
        strcpy(key_backup, key);
        value = i * 2;
        tbl_set(tbl, key, &i);
        tbl_set(tbl, key, &value);
        assert(strcmp(key, key_backup) == 0);
        assert(value == i * 2);
        assert(tbl_size(tbl) == i + 1);
    }
    for (int i = 0; i < size; i++) {
        sprintf(key, "key%d", i);
        strcpy(key_backup, key);
        value = -1;
        tbl_get(tbl, key, &value);
        assert(strcmp(key, key_backup) == 0);
        assert(value == i * 2);
        assert(tbl_size(tbl) == size);
    }

    tbl_kill(tbl);

    printf("pass\n");
}

void func_test3() {
    int key_len = 1024;

    printf("tbl_contain... ");

    table *tbl = tbl_init(sizeof(int));
    int n;
    char *long_key = (char *)malloc(sizeof(char) * key_len);
    for (int i = 0; i < key_len - 1; i++) {
        long_key[i] = 'a';
    }
    long_key[key_len - 1] = 0;

    tbl_set(tbl, "", &n);
    tbl_set(tbl, "key", &n);
    tbl_set(tbl, "  key  ", &n);
    tbl_set(tbl, long_key, &n);

    assert(tbl_contain(tbl, ""));
    assert(tbl_contain(tbl, "key"));
    assert(tbl_contain(tbl, "  key  "));
    assert(tbl_contain(tbl, long_key));

    long_key[key_len - 2] = 0;

    assert(!tbl_contain(tbl, " "));
    assert(!tbl_contain(tbl, "keykey"));
    assert(!tbl_contain(tbl, "  key"));
    assert(!tbl_contain(tbl, "key  "));
    assert(!tbl_contain(tbl, long_key));

    tbl_kill(tbl);
    free(long_key);

    printf("pass\n");
}

void func_test4() {
    printf("tbl_remove... ");

    table *tbl = tbl_init(sizeof(int));
    int n;
    tbl_set(tbl, "key1", &n);
    tbl_set(tbl, "key2", &n);
    tbl_set(tbl, "key3", &n);
    tbl_set(tbl, "key4", &n);

    tbl_remove(tbl, "key2");
    tbl_remove(tbl, "key3");
    assert(tbl_contain(tbl, "key1"));
    assert(tbl_contain(tbl, "key4"));
    assert(!tbl_contain(tbl, "key2"));
    assert(!tbl_contain(tbl, "key3"));

    tbl_kill(tbl);

    printf("pass\n");
}

void func_test5() {
    int size = 1000;

    printf("tbl_size, tbl_is_empty... ");

    table *tbl = tbl_init(sizeof(int));
    char key[16];

    assert(tbl_size(tbl) == 0);
    assert(tbl_is_empty(tbl));

    for (int i = 0; i < size; i++) {
        sprintf(key, "key%d", i);
        tbl_set(tbl, key, &i);
        assert(tbl_size(tbl) == i + 1);
        assert(!tbl_is_empty(tbl));
    }
    for (int i = 0; i < size - 1; i++) {
        sprintf(key, "key%d", i);
        tbl_remove(tbl, key);
        assert(tbl_size(tbl) == size - i - 1);
        assert(!tbl_is_empty(tbl));
    }
    sprintf(key, "key%d", size - 1);
    tbl_remove(tbl, key);
    assert(tbl_size(tbl) == 0);
    assert(tbl_is_empty(tbl));

    tbl_kill(tbl);

    printf("pass\n");
}

void func_test6() {
    int size = 100;

    printf("tbl_keys... ");

    table *tbl = tbl_init(sizeof(int));
    char key[16];
    for (int i = 0; i < size; i++) {
        sprintf(key, "key%d", i);
        tbl_set(tbl, key, &i);
    }

    char **keys = tbl_keys(tbl);
    for (int i = 0; i < size; i++) {
        sprintf(key, "key%d", i);
        int j;
        for (j = 0; j < size; j++) {
            if (strcmp(key, keys[j]) == 0) {
                break;
            }
        }
        if (j == size) {
            assert(0);
        }
    }

    for (int i = 0; i < size; i++) {
        free(keys[i]);
    }
    free(keys);
    tbl_kill(tbl);

    printf("pass\n");
}

void time_test1() {
    int T = 100000;
    int key_len = 10;

    printf("%d tbl_get and tbl_set... ", T);

    table *tbl = tbl_init(sizeof(int));

    char **keys = (char **)malloc(sizeof(char *) * T);
    for (int i = 0; i < T; i++) {
        keys[i] = (char *)malloc(sizeof(char) * (key_len + 1));
        for (int j = 0; j < key_len; j++) {
            keys[i][j] = rand() % 256;
        }
        keys[i][key_len] = 0;
    }
    int *values = (int *)malloc(sizeof(int) * T);
    for (int i = 0; i < T; i++) {
        values[i] = rand();
    }

    clock_t start = clock();

    for (int i = 0; i < T; i++) {
        tbl_set(tbl, keys[i], &values[i]);
    }
    for (int i = 0; i < T; i++) {
        tbl_get(tbl, keys[i], &values[i]);
    }

    clock_t end = clock();
    tbl_kill(tbl);
    for (int i = 0; i < T; i++) {
        free(keys[i]);
    }
    free(keys);
    free(values);

    printf("%f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

int main() {
    printf("=== start table tests ===\n");

    func_test1();
    func_test2();
    func_test3();
    func_test4();
    func_test5();
    func_test6();

    time_test1();

    printf("=== pass table tests ===\n");

    return 0;
}
