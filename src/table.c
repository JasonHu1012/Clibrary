#include "table.h"
#include "llist.h"
#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_ARR_SIZE 32
#define INCREASE_SIZE_THRESHOLD 10

struct table {
    // data
    list *arr;
    // indicates who the llist actually belongs to
    list *belong;
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
    ret->arr = lst_init(sizeof(llist *));
    ret->belong = lst_init(sizeof(int));
    lst_set_size(ret->arr, INITIAL_ARR_SIZE);
    lst_set_size(ret->belong, INITIAL_ARR_SIZE);
    for (int i = 0; i < INITIAL_ARR_SIZE; i++) {
        llist *llst = llst_init(sizeof(kv_pair *));
        lst_set(ret->arr, i, &llst);
        lst_set(ret->belong, i, &i);
    }
    ret->size = 0;
    ret->width = width;
    return ret;
}

void tbl_kill(table *tbl) {
    for (int i = 0; i < lst_size(tbl->arr); i++) {
        int belong;
        lst_get(tbl->belong, i, &belong);
        if (i != belong) {
            // the llist belongs to others, don't kill it
            continue;
        }

        llist *llst;
        lst_get(tbl->arr, i, &llst);

        if (!llst_is_empty(llst)) {
            llist_iter *llst_it = llst_it_init(llst, LLST_HEAD);
            do {
                kv_pair *pair;
                llst_it_get(llst_it, &pair);

                kill_kv_pair(pair);
            } while (llst_it_move(llst_it, LLST_TAIL));

            llst_it_kill(llst_it);
        }

        llst_kill(llst);
    }

    lst_kill(tbl->arr);
    lst_kill(tbl->belong);
    free(tbl);
}

static int hash(char *key, int arr_len) {
    const int MOD = 1000000007;

    int len = strlen(key);

    int ret = 0;
    for (int i = 0; i < len; i += sizeof(int)) {
        int cur = 0;
        memcpy(&cur, key + i, len - i < sizeof(int) ? len - i : sizeof(int));
        ret ^= cur;
    }

    ret %= MOD;
    if (ret < 0) {
        ret += MOD;
    }

    ret %= arr_len;
    if (ret < 0) {
        ret += arr_len;
    }

    return ret;
}

static void resize(table *tbl, char *key) {
    // size only increases

    llist *llst;
    lst_get(tbl->arr, hash(key, lst_size(tbl->arr)), &llst);

    if (llst_size(llst) <= INCREASE_SIZE_THRESHOLD) {
        return;
    }

    // clean up the llist
    int belong;
    lst_get(tbl->belong, hash(key, lst_size(tbl->arr)), &belong);

    // don't use llist_iter to avoid removing while iterating
    int size = llst_size(llst);
    for (int i = 0; i < size; i++) {
        kv_pair *pair;
        llst_remove(llst, &pair, LLST_HEAD);

        int index = hash(pair->key, lst_size(tbl->arr));
        if (index == belong) {
            // the pair is at the correct llist, put it back
            llst_append(llst, &pair, LLST_TAIL);
            continue;
        }

        int target_belong;
        lst_get(tbl->belong, index, &target_belong);
        if (index != target_belong) {
            // `tbl->arr[index]` points at other's llist
            // create a new llist for it
            llist *new_llst = llst_init(sizeof(kv_pair *));
            lst_set(tbl->arr, index, &new_llst);
            lst_set(tbl->belong, index, &index);
        }

        // move the pair to the correct llist
        llist *target_llst;
        lst_get(tbl->arr, index, &target_llst);
        llst_append(target_llst, &pair, LLST_TAIL);
    }

    if (llst_size(llst) <= INCREASE_SIZE_THRESHOLD) {
        return;
    }

    // need to increase size, but clean up next time
    int old_size = lst_size(tbl->arr);
    lst_set_size(tbl->arr, old_size * 2);
    lst_set_size(tbl->belong, old_size * 2);
    for (int i = old_size; i < old_size * 2; i++) {
        llist *target_llst;
        int belong;
        lst_get(tbl->arr, i - old_size, &target_llst);
        lst_set(tbl->arr, i, &target_llst);
        lst_get(tbl->belong, i - old_size, &belong);
        lst_set(tbl->belong, i, &belong);
    }
}

void tbl_get(table *tbl, char *key, void *dst) {
    resize(tbl, key);

    llist *llst;
    lst_get(tbl->arr, hash(key, lst_size(tbl->arr)), &llst);

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
    resize(tbl, key);

    llist *llst;
    lst_get(tbl->arr, hash(key, lst_size(tbl->arr)), &llst);

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
    llist *llst;
    lst_get(tbl->arr, hash(key, lst_size(tbl->arr)), &llst);

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
    llist *llst;
    lst_get(tbl->arr, hash(key, lst_size(tbl->arr)), &llst);
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
    for (int i = 0; i < lst_size(tbl->arr); i++) {
        int belong;
        lst_get(tbl->belong, i, &belong);
        if (i != belong) {
            // the llist belongs to others, don't iterate it
            continue;
        }

        llist *llst;
        lst_get(tbl->arr, i, &llst);

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

#undef INITIAL_ARR_SIZE
#undef INCREASE_SIZE_THRESHOLD
