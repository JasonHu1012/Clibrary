#include "queue.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

queue *que_init(int width) {
    assert(width > 0);
    queue *output = (queue *)calloc(1, sizeof(queue));
    output->width = width;
    return output;
}

void que_push(queue *q, void *src) {
    assert(src);
    lnode *n = lnd_init(src, q->width);
    if (!q->tail) {
        q->head = n;
        q->tail = n;
    }
    else {
        q->tail->next = n;
        q->tail = n;
    }
    q->size++;
}

void que_pop(queue *q, void *dst) {
    if (dst) {
        memcpy(dst, q->head->content, q->width);
    }
    lnode *n = q->head->next;
    lnd_kill(q->head);
    q->head = n;
    if (!q->head) {
        q->tail = NULL;
    }
    q->size--;
}

void que_top(queue *q, void *dst) {
    assert(dst);
    memcpy(dst, q->head->content, q->width);
}

int que_size(queue *q) {
    return q->size;
}

void que_kill(queue *q) {
    while (q->head) {
        lnode *n = q->head->next;
        lnd_kill(q->head);
        q->head = n;
    }
    free(q);
}
