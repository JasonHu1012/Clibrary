#ifndef _DEQUE_H
#define _DEQUE_H

#include <stdbool.h>

typedef struct deque deque;

// initialize a deque of type occupying `width` byte(s)
deque *deq_init(int width);

// kill `deq`
void deq_kill(deque *deq);

// return size of `deq`
int deq_size(deque *deq);

// return whether `deq` is empty
bool deq_is_empty(deque *deq);

// push (copy) `src` to head of `deq`
void deq_push_head(deque *deq, void *src);

// pop head of `deq` to `dst`
// `dst` can be NULL
void deq_pop_head(deque *deq, void *dst);

// push (copy) `src` to tail of `deq`
void deq_push_tail(deque *deq, void *src);

// pop tail of `deq` to `dst`
// `dst` can be NULL
void deq_pop_tail(deque *deq, void *dst);

// copy head of `deq` to `dst`
void deq_head(deque *deq, void *dst);

// copy tail of `deq` to `dst`
void deq_tail(deque *deq, void *dst);

#endif
