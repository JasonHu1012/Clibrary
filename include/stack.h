#ifndef _STACK_H
#define _STACK_H

#include <stdbool.h>

typedef struct stack stack;

// initialize a stack of type occupying `width` byte(s)
stack *stk_init(int width);

// kill `stk`
void stk_kill(stack *stk);

// return size of `stk`
int stk_size(stack *stk);

// return whether `stk` is empty
bool stk_is_empty(stack *stk);

// push (copy) `src` to `stk`
void stk_push(stack *stk, void *src);

// pop top element of `stk` to `dst`
// `dst` can be NULL
void stk_pop(stack *stk, void *dst);

// copy top element of `stk` to `dst`
void stk_top(stack *stk, void *dst);

#endif
