#include "deque.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    void *data;
    struct node *next;
    struct node *prev;
} node;

// linked list of array
struct deque {
    node *head;
    node *tail;
    // both `start` and `end` fall in [0, len)
    int start;
    int end;
    int width;
    int size;
};

const int SINGLE_NODE_SIZE = 1024;

static node *init_node(int width) {
    node *ret = (node *)malloc(sizeof(node));
    ret->data = malloc(width * SINGLE_NODE_SIZE);
    ret->next = NULL;
    ret->prev = NULL;
    return ret;
}

static void kill_node(node *n) {
    free(n->data);
    free(n);
}

deque *deq_init(int width) {
    assert(width > 0);

    deque *ret = (deque *)malloc(sizeof(deque));
    ret->head = init_node(width);
    ret->tail = ret->head;
    ret->start = 0;
    ret->end = 0;
    ret->width = width;
    ret->size = 0;
    return ret;
}

void deq_kill(deque *deq) {
    while (deq->head) {
        node *old_head = deq->head;
        deq->head = deq->head->next;

        kill_node(old_head);
    }
    free(deq);
}

int deq_size(deque *deq) {
    return deq->size;
}

bool deq_is_empty(deque *deq) {
    return deq->size == 0;
}

void deq_push_head(deque *deq, void *src) {
    if (deq->start == 0) {
        node *old_head = deq->head;
        deq->head = init_node(deq->width);

        deq->head->next = old_head;
        old_head->prev = deq->head;

        deq->start = SINGLE_NODE_SIZE;
    }

    deq->start--;
    memcpy((char *)deq->head->data + deq->width * deq->start, src, deq->width);

    deq->size++;
}

void deq_pop_head(deque *deq, void *dst) {
    assert(deq->size > 0);

    if (dst) {
        memcpy(dst, (char *)deq->head->data + deq->width * deq->start, deq->width);
    }

    if (++deq->start == SINGLE_NODE_SIZE) {
        node *old_head = deq->head;
        deq->head = deq->head->next;
        deq->start = 0;

        kill_node(old_head);
    }

    deq->size--;
}

void deq_push_tail(deque *deq, void *src) {
    if (deq->end == SINGLE_NODE_SIZE) {
        node *old_tail = deq->tail;
        deq->tail = init_node(deq->width);

        deq->tail->prev = old_tail;
        old_tail->next = deq->tail;

        deq->end = 0;
    }

    memcpy((char *)deq->tail->data + deq->width * deq->end, src, deq->width);
    deq->end++;

    deq->size++;
}

void deq_pop_tail(deque *deq, void *dst) {
    assert(deq->size > 0);

    if (deq->end-- == 0) {
        node *old_tail = deq->tail;
        deq->tail = deq->tail->prev;
        deq->end = SINGLE_NODE_SIZE - 1;

        kill_node(old_tail);
    }

    if (dst) {
        memcpy(dst, (char *)deq->tail->data + deq->width * deq->end, deq->width);
    }

    deq->size--;
}

void deq_head(deque *deq, void *dst) {
    memcpy(dst, (char *)deq->head->data + deq->width * deq->start, deq->width);
}

void deq_tail(deque *deq, void *dst) {
    memcpy(
        dst,
        deq->end == 0 ?
            (char *)deq->tail->prev->data + deq->width * (SINGLE_NODE_SIZE - 1) :
            (char *)deq->tail->data + deq->width * (deq->end - 1),
        deq->width);
}
