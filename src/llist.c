#include "llist.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct node {
    void *data;
    struct node *next;
    struct node *prev;
} node;

struct llist {
    // both `head` and `tail` point at dummy nodes
    node *head;
    node *tail;
    int width;
    int size;
};

struct llist_iter {
    node *cur;
    llist *belong;
};

static node *init_node(int width) {
    node *ret = (node *)malloc(sizeof(node));
    ret->data = malloc(width);
    ret->next = NULL;
    ret->prev = NULL;
    return ret;
}

static void kill_node(node *n) {
    free(n->data);
    free(n);
}

llist *llst_init(int width) {
    assert(width > 0);

    llist *ret = (llist *)malloc(sizeof(llist));
    ret->head = init_node(0);
    ret->tail = init_node(0);
    ret->head->next = ret->tail;
    ret->tail->prev = ret->head;
    ret->width = width;
    ret->size = 0;
    return ret;
}

void llst_kill(llist *llst) {
    while (llst->head) {
        node *old_head = llst->head;
        llst->head = llst->head->next;

        kill_node(old_head);
    }
    free(llst);
}

llist_iter *llst_it_init(llist *llst, llist_way way) {
    assert(llst->size > 0);

    llist_iter *ret = (llist_iter *)malloc(sizeof(llist_iter));
    ret->cur = way == LLST_HEAD ? llst->head->next : llst->tail->prev;
    ret->belong = llst;
    return ret;
}

void llst_it_kill(llist_iter *llst_it) {
    free(llst_it);
}

void llst_append(llist *llst, void *src, llist_way way) {
    node *new_node = init_node(llst->width);
    memcpy(new_node->data, src, llst->width);

    if (way == LLST_HEAD) {
        node *old_next = llst->head->next;

        llst->head->next = new_node;
        new_node->prev = llst->head;

        old_next->prev = new_node;
        new_node->next = old_next;
    }
    else {
        node *old_prev = llst->tail->prev;

        llst->tail->prev = new_node;
        new_node->next = llst->tail;

        old_prev->next = new_node;
        new_node->prev = old_prev;
    }

    llst->size++;
}

void llst_remove(llist *llst, void *dst, llist_way way) {
    assert(llst->size > 0);

    if (way == LLST_HEAD) {
        if (dst) {
            memcpy(dst, llst->head->next->data, llst->width);
        }

        node *new_next = llst->head->next->next;

        kill_node(llst->head->next);

        llst->head->next = new_next;
        new_next->prev = llst->head;
    }
    else {
        if (dst) {
            memcpy(dst, llst->tail->prev->data, llst->width);
        }

        node *new_prev = llst->tail->prev->prev;

        kill_node(llst->tail->prev);

        llst->tail->prev = new_prev;
        new_prev->next = llst->tail;
    }

    llst->size--;
}

int llst_size(llist *llst) {
    return llst->size;
}

bool llst_is_empty(llist *llst) {
    return llst->size == 0;
}

bool llst_it_move(llist_iter *llst_it, llist_way way) {
    if (way == LLST_HEAD) {
        llst_it->cur = llst_it->cur->prev;

        if (!llst_it->cur->prev) {
            llst_it->cur = llst_it->cur->next;
            return false;
        }
    }
    else {
        llst_it->cur = llst_it->cur->next;

        if (!llst_it->cur->next) {
            llst_it->cur = llst_it->cur->prev;
            return false;
        }
    }

    return true;
}

void llst_it_move_to(llist_iter *llst_it, llist_way way) {
    assert(llst_it->belong->size > 0);

    llst_it->cur = way == LLST_HEAD ? llst_it->belong->head->next : llst_it->belong->tail->prev;
}

void llst_it_get(llist_iter *llst_it, void *dst) {
    memcpy(dst, llst_it->cur->data, llst_it->belong->width);
}

void llst_it_set(llist_iter *llst_it, void *src) {
    memcpy(llst_it->cur->data, src, llst_it->belong->width);
}

void llst_it_insert(llist_iter *llst_it, void *src, llist_way way) {
    node *new_node = init_node(llst_it->belong->width);
    memcpy(new_node->data, src, llst_it->belong->width);

    if (way == LLST_HEAD) {
        node *old_prev = llst_it->cur->prev;

        old_prev->next = new_node;
        new_node->prev = old_prev;

        llst_it->cur->prev = new_node;
        new_node->next = llst_it->cur;
    }
    else {
        node *old_next = llst_it->cur->next;

        llst_it->cur->next = new_node;
        new_node->prev = llst_it->cur;

        old_next->prev = new_node;
        new_node->next = old_next;
    }

    llst_it->belong->size++;
}

void llst_it_remove(llist_iter *llst_it, void *dst, llist_way way) {
    assert(llst_it->belong->size > 0);

    if (dst) {
        memcpy(dst, llst_it->cur->data, llst_it->belong->width);
    }

    node *new_cur;
    if (way == LLST_HEAD) {
        new_cur = llst_it->cur->prev;
        if (!new_cur->prev) {
            new_cur = llst_it->cur->next;
        }
    }
    else {
        new_cur = llst_it->cur->next;
        if (!new_cur->next) {
            new_cur = llst_it->cur->prev;
        }
    }

    llst_it->cur->prev->next = llst_it->cur->next;
    llst_it->cur->next->prev = llst_it->cur->prev;

    kill_node(llst_it->cur);

    llst_it->cur = new_cur;

    llst_it->belong->size--;
}
