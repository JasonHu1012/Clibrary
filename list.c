#include "list.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

struct list {
    list_node *dummy;
    int size;
    int width;
};

struct list_node {
    lnode *inherit;
    list *belong;
    int uref; // user reference: 0 or 1
    int oref; // other reference: list(one) and iter(many)
              // node will be removed from list when oref is 0
              // node will be killed when uref and oref are both 0
};

struct list_iter {
    list_node *cur;
    bool reverse;
    bool end;
};

list *lst_init(int width) {
    assert(width > 0);
    list *output = (list *)malloc(sizeof(list));
    output->dummy = lst_init_node(NULL, 0);
    output->dummy->uref = 0;
    output->dummy->oref++;
    output->dummy->belong = output;
    output->dummy->inherit->prev = output->dummy->inherit;
    output->dummy->inherit->next = output->dummy->inherit;
    output->size = 0;
    output->width = width;
    return output;
}

list_node *lst_init_node(void *src, int width) {
    // width = 0 to be dummy node, used in lst_init
    assert(width >= 0);
    list_node *output = (list_node *)malloc(sizeof(list_node));
    void *temp = malloc(sizeof(list_node *) + width);
    memcpy(temp, &output, sizeof(list_node *));
    if (src) {
        memcpy((char *)temp + sizeof(list_node *), src, width);
    }
    // content = [list_node *][real content]
    output->inherit = lnd_init(temp, sizeof(list_node *) + width);
    free(temp);
    output->belong = NULL;
    output->uref = 1;
    output->oref = 0;
    return output;
}

list_iter *lst_init_iter(list *l, bool reverse) {
    list_iter *output = (list_iter *)malloc(sizeof(list_iter));
    output->cur = l->dummy;
    l->dummy->oref++;
    output->reverse = reverse;
    output->end = false;
    return output;
}

static void insert(list *l, void *src, list_node *n, lnode *a, lnode *b) {
    assert((src || n) && (!src || !n));
    if (src) {
        n = lst_init_node(src, l->width);
        n->uref = 0; // user doesn't reference the node
    }
    assert(!n->belong);
    assert(!n->inherit->prev && !n->inherit->next);
    n->belong = l;
    n->oref++;
    n->inherit->next = b;
    n->inherit->prev = a;
    b->prev = n->inherit;
    a->next = n->inherit;
    l->size++;
}

void lst_add_head(list *l, void *src, list_node *n) {
    insert(l, src, n, l->dummy->inherit, l->dummy->inherit->next);
}

void lst_add_tail(list *l, void *src, list_node *n) {
    insert(l, src, n, l->dummy->inherit->prev, l->dummy->inherit);
}

void lst_add_iter_next(list_iter *i, void *src, list_node *n) {
    insert(i->cur->belong, src, n, i->cur->inherit, i->cur->inherit->next);
}

void lst_add_iter_prev(list_iter *i, void *src, list_node *n) {
    insert(i->cur->belong, src, n, i->cur->inherit->prev, i->cur->inherit);
}

static void check(list_node *n) {
    if (n->oref == 0) {
        n->inherit->prev->next = n->inherit->next;
        n->inherit->next->prev = n->inherit->prev;
        n->inherit->next = NULL;
        n->inherit->prev = NULL;
        if (n->uref == 0) {
            lst_kill_node(n);
        }
    }
}

static void remove_from_list(list_node *n) {
    list *l = n->belong;
    l->size--;
    n->belong = NULL;
    n->oref--;
    check(n);
}

void lst_remove(list_node *n) {
    assert(n->belong);
    remove_from_list(n);
}

void lst_iter_remove(list_iter *i) {
    assert(i->cur->belong);
    assert(i->cur != i->cur->belong->dummy);
    remove_from_list(i->cur);
}

int lst_size(list *l) {
    return l->size;
}

bool lst_iter_next(list_iter *i, void *dst) {
    if (i->end) {
        return false;
    }
    do {
        list_node *last = i->cur;
        lnode *next_lnode = i->reverse ? i->cur->inherit->prev : i->cur->inherit->next;
        memcpy(&i->cur, next_lnode->content, sizeof(list_node *));
        last->oref--;
        i->cur->oref++;
        check(last);
    } while (!i->cur->belong);
    if (i->cur == i->cur->belong->dummy) {
        i->end = true;
        return false;
    }
    if (dst) {
        memcpy(dst, (char *)i->cur->inherit->content + sizeof(list_node *), i->cur->belong->width);
    }
    return true;
}

void lst_iter_reset(list_iter *i) {
    i->end = false;
    list_node *last = i->cur;
    i->cur = i->cur->belong->dummy;
    last->oref--;
    i->cur->oref++;
    check(last);
}

void lst_kill_iter(list_iter *i) {
    i->cur->oref--;
    check(i->cur);
    free(i);
}

void lst_get_node(list_node *n, void *dst, int width) {
    memcpy(dst, (char *)n->inherit->content + sizeof(list_node *), width);
}

void lst_set_node(list_node *n, void *src, int width) {
    memcpy((char *)n->inherit->content + sizeof(list_node *), src, width);
}

void lst_kill_node(list_node *n) {
    if (n->uref) {
        n->uref = 0;
        if (n->oref) {
            return;
        }
    }
    assert(!n->oref);
    lnd_kill(n->inherit);
    free(n);
}

void lst_kill(list *l) {
    while (l->dummy->inherit != l->dummy->inherit->next) {
        list_node *n;
        memcpy(&n, l->dummy->inherit->next->content, sizeof(list_node *));
        if (n->belong) {
            n->oref = 1;
            remove_from_list(n);
        }
        else {
            n->oref = 0;
            check(n);
        }
    }
    l->dummy->oref = 0;
    lst_kill_node(l->dummy);
    free(l);
}
