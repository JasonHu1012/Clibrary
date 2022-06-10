#ifndef _STACK_H
#define _STACK_H

#include "lnode.h"

typedef struct stack stack;

stack *stk_init(int width);
void stk_push(stack *s, void *src);
void stk_pop(stack *s, void *dst);
void stk_top(stack *s, void *dst);
int stk_size(stack *s);
void stk_kill(stack *s);

#endif
