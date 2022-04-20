#ifndef QUEUE_H
#define QUEUE_H

#include "lnode.h"

typedef struct queue queue;

queue *que_init(int width);
void que_push(queue *q, void *src);
void que_pop(queue *q, void *dst);
void que_top(queue *q, void *dst);
int que_size(queue *q);
void que_kill(queue *q);

struct queue {
    int width;
    int size;
    lnode *head;
    lnode *tail;
};

#endif
