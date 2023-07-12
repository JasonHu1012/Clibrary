#include "stack.h"
#include "list.h"
#include <stdlib.h>
#include <assert.h>

struct stack {
    list *lst;
};

stack *stk_init(int width) {
    assert(width > 0);

    stack *ret = (stack *)malloc(sizeof(stack));
    ret->lst = lst_init(width);
    return ret;
}

void stk_kill(stack *stk) {
    lst_kill(stk->lst);
    free(stk);
}

int stk_size(stack *stk) {
    return lst_size(stk->lst);
}

bool stk_is_empty(stack *stk) {
    return lst_is_empty(stk->lst);
}

void stk_push(stack *stk, void *src) {
    lst_append(stk->lst, src);
}

void stk_pop(stack *stk, void *dst) {
    lst_pop(stk->lst, dst);
}

void stk_top(stack *stk, void *dst) {
    lst_get(stk->lst, lst_size(stk->lst) - 1, dst);
}
