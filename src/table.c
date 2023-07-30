// there's no resize mechanism in this implementation

#include "table.h"
#include "llist.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ARR_SIZE 256

struct table {
    llist *arr[ARR_SIZE];
    int size;
    int width;
};

typedef struct kv_pair {
    char *key;
    void *value;
} kv_pair;

static kv_pair *init_kv_pair(char *key, void *value, int width) {
    kv_pair *ret = (kv_pair *)malloc(sizeof(kv_pair));
    ret->key = (char *)malloc(sizeof(char) * (strlen(key) + 1));
    ret->value = malloc(width);
    strcpy(ret->key, key);
    memcpy(ret->value, value, width);
    return ret;
}

static void kill_kv_pair(kv_pair *pair) {
    free(pair->key);
    free(pair->value);
    free(pair);
}

table *tbl_init(int width) {
    assert(width > 0);

    table *ret = (table *)malloc(sizeof(table));
    for (int i = 0; i < ARR_SIZE; i++) {
        ret->arr[i] = llst_init(sizeof(kv_pair *));
    }
    ret->size = 0;
    ret->width = width;
    return ret;
}

void tbl_kill(table *tbl) {
    for (int i = 0; i < ARR_SIZE; i++) {
        llist *llst = tbl->arr[i];

        if (!llst_is_empty(llst)) {
            llist_iter *llst_it = llst_it_init(llst, LLST_HEAD);
            do {
                kv_pair *pair;
                llst_it_get(llst_it, &pair);

                kill_kv_pair(pair);
            } while (llst_it_move(llst_it, LLST_TAIL));
        }

        llst_kill(llst);
    }

    free(tbl);
}

static int hash(char *key) {
    int len = strlen(key);

    int ret = 0;
    for (int i = 0; i < len; i += sizeof(int)) {
        int cur;
        memcpy(&cur, key + i, len - i < sizeof(int) ? len - i : sizeof(int));
        ret ^= cur;
    }

    ret %= ARR_SIZE;
    if (ret < 0) {
        ret += ARR_SIZE;
    }
    
    return ret;
}

void tbl_get(table *tbl, char *key, void *dst) {
    llist *llst = tbl->arr[hash(key)];

    if (llst_is_empty(llst)) {
        return;
    }

    llist_iter *llst_it = llst_it_init(llst, LLST_HEAD);
    do {
        kv_pair *pair;
        llst_it_get(llst_it, &pair);

        if (strcmp(pair->key, key) == 0) {
            memcpy(dst, pair->value, tbl->width);
            break;
        }
    } while (llst_it_move(llst_it, LLST_TAIL));

    llst_it_kill(llst_it);
}

void tbl_set(table *tbl, char *key, void *src) {
    llist *llst = tbl->arr[hash(key)];

    if (!llst_is_empty(llst)) {
        llist_iter *llst_it = llst_it_init(llst, LLST_HEAD);
        do {
            kv_pair *pair;
            llst_it_get(llst_it, &pair);

            if (strcmp(pair->key, key) == 0) {
                // remove the old pair and node
                kill_kv_pair(pair);
                llst_it_remove(llst_it, NULL, LLST_TAIL);

                tbl->size--;
                break;
            }
        } while (llst_it_move(llst_it, LLST_TAIL));

        llst_it_kill(llst_it);
    }

    // append new pair at head
    // the newly set key can be accessed faster
    kv_pair *pair = init_kv_pair(key, src, tbl->width);
    llst_append(llst, &pair, LLST_HEAD);

    tbl->size++;
}

bool tbl_contain(table *tbl, char *key) {
    llist *llst = tbl->arr[hash(key)];

    if (llst_is_empty(llst)) {
        return false;
    }

    bool is_found = false;
    llist_iter *llst_it = llst_it_init(llst, LLST_HEAD);
    do {
        kv_pair *pair;
        llst_it_get(llst_it, &pair);

        if (strcmp(pair->key, key) == 0) {
            is_found = true;
            break;
        }
    } while (llst_it_move(llst_it, LLST_TAIL));

    llst_it_kill(llst_it);

    return is_found;
}

void tbl_remove(table *tbl, char *key) {
    llist *llst = tbl->arr[hash(key)];
    bool is_found = false;

    if (llst_is_empty(llst)) {
        assert(is_found);
    }

    llist_iter *llst_it = llst_it_init(llst, LLST_HEAD);
    do {
        kv_pair *pair;
        llst_it_get(llst_it, &pair);

        if (strcmp(pair->key, key) == 0) {
            is_found = true;

            kill_kv_pair(pair);
            llst_it_remove(llst_it, NULL, LLST_TAIL);

            tbl->size--;
            break;
        }
    } while (llst_it_move(llst_it, LLST_TAIL));

    assert(is_found);

    llst_it_kill(llst_it);
}

int tbl_size(table *tbl) {
    return tbl->size;
}

bool tbl_is_empty(table *tbl) {
    return tbl->size == 0;
}

char **tbl_keys(table *tbl) {
    char **ret = (char **)malloc(sizeof(char *) * tbl->size);
    int ret_i = 0;
    for (int i = 0; i < ARR_SIZE; i++) {
        llist *llst = tbl->arr[i];

        if (llst_is_empty(llst)) {
            continue;
        }

        llist_iter *llst_it = llst_it_init(llst, LLST_HEAD);
        do {
            kv_pair *pair;
            llst_it_get(llst_it, &pair);

            ret[ret_i] = (char *)malloc(sizeof(char) * (strlen(pair->key) + 1));
            strcpy(ret[ret_i], pair->key);

            ret_i++;
        } while (llst_it_move(llst_it, LLST_TAIL));

        llst_it_kill(llst_it);
    }
    return ret;
}
