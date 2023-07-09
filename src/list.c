#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct list {
    void *data;
    int width;
    int size;
    // always larger than `size`
    int real_size;
};

int INITIAL_REAL_SIZE = 16;

list *lst_init(int width) {
    assert(width > 0);

    list *ret = (list *)malloc(sizeof(list));
    ret->data = malloc(width * INITIAL_REAL_SIZE);
    ret->width = width;
    ret->size = 0;
    ret->real_size = INITIAL_REAL_SIZE;
    return ret;
}

void lst_kill(list *lst) {
    free(lst->data);
    free(lst);
}

static void increase_real_size(list *lst) {
    if (lst->size < lst->real_size) {
        return;
    }

    while (lst->size >= lst->real_size) {
        lst->real_size <<= 1;
    }

    lst->data = realloc(lst->data, lst->width * lst->real_size);
}

static void decrease_real_size(list *lst) {
    if (lst->size >= lst->real_size >> 1) {
        return;
    }

    while (lst->size < lst->real_size) {
        lst->real_size >>= 1;
    }
    lst->real_size <<= 1;

    if (!lst->real_size) {
        lst->real_size = 1;
    }

    lst->data = realloc(lst->data, lst->width * lst->real_size);
}

void lst_set_size(list *lst, int size) {
    assert(size >= 0);

    lst->size = size;

    lst->size < lst->real_size ? decrease_real_size(lst) : increase_real_size(lst);
}

int lst_size(list *lst) {
    return lst->size;
}

bool lst_is_empty(list *lst) {
    return lst->size == 0;
}

void lst_get(list *lst, int i, void *dst) {
    assert(i >= 0 && i < lst->size);

    memcpy(dst, (char *)lst->data + lst->width * i, lst->width);
}

void lst_set(list *lst, int i, void *src) {
    assert(i >= 0 && i < lst->size);

    memcpy((char *)lst->data + lst->width * i, src, lst->width);
}

void lst_append(list *lst, void *src) {
    memcpy((char *)lst->data + lst->width * lst->size++, src, lst->width);

    increase_real_size(lst);
}

void lst_pop(list *lst, void *dst) {
    assert(lst->size > 0);

    lst->size--;
    if (dst) {
        memcpy(dst, (char *)lst->data + lst->width * lst->size, lst->width);
    }

    decrease_real_size(lst);
}

void lst_insert(list *lst, int i, void *src) {
    assert(i >= 0 && i <= lst->size);

    if (i == lst->size) {
        lst_append(lst, src);
        return;
    }

    memmove((char *)lst->data + lst->width * (i + 1),
            (char *)lst->data + lst->width * i,
            lst->width * (lst->size - i));
    memcpy((char *)lst->data + lst->width * i, src, lst->width);
    lst->size++;

    increase_real_size(lst);
}

void lst_remove(list *lst, int i, void *dst) {
    assert(i >= 0 && i < lst->size);

    if (dst) {
        memcpy(dst, (char *)lst->data + lst->width * i, lst->width);
    }

    memmove((char *)lst->data + lst->width * i,
            (char *)lst->data + lst->width * (i + 1),
            lst->width * (lst->size - i - 1));
    lst->size--;
}
