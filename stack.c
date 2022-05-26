#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

stack *stk_init(int width) {
    assert(width > 0);
    stack *output = (stack *)calloc(1, sizeof(stack));
    output->width = width;
    return output;
}

void stk_push(stack *s, void *src) {
    lnode *n = lnd_init(src, s->width);
    n->next = s->top;
    s->top = n;
    s->size++;
}
    
void stk_pop(stack *s, void *dst) {
    if (dst) {
        memcpy(dst, s->top->content, s->width);
    }
    lnode *n = s->top->next;
    lnd_kill(s->top);
    s->top = n;
    s->size--;
}

void stk_top(stack *s, void *dst) {
    assert(dst);
    memcpy(dst, s->top->content, s->width);
}

int stk_size(stack *s) {
    return s->size;
}

void stk_kill(stack *s) {
    while (s->top) {
        lnode *n = s->top->next;
        lnd_kill(s->top);
        s->top = n;
    }
    free(s);
}
