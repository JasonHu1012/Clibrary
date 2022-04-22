#include "list.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

list *lst_init(int width) {
    assert(width > 0);
    list *output = (list *)malloc(sizeof(list));
    output->dummy = lst_init_node(NULL, 0);
    output->dummy->inherit->prev = output->dummy->inherit;
    output->dummy->inherit->next = output->dummy->inherit;
    output->iter = NULL;
    output->size = 0;
    output->width = width;
    return output;
}

list_node *lst_init_node(void *src, int width) {
    assert(width >= 0);
    if (width) {
        assert(src);
    }
    list_node *output = (list_node *)malloc(sizeof(list_node));
    void *temp = malloc(sizeof(width) + sizeof(list_node *));
    memcpy(temp, src, width);
    memcpy((char *)temp + width, &output, sizeof(list_node *));
    // content = [real content][list_node *]
    output->inherit = lnd_init(temp, width + sizeof(list_node *));
    free(temp);
    output->width = width;
    output->belong = NULL;
    return output;
}

void lst_append(list *l, void *src, list_node *n) {
    assert((src || n) && (!src || !n));
    if (src) {
        n = lst_init_node(src, l->width);
    }
    assert(l->width == n->width);
    n->belong = l;
    n->inherit->prev = l->dummy->inherit->prev;
    n->inherit->next = l->dummy->inherit;
    l->dummy->inherit->prev->next = n->inherit;
    l->dummy->inherit->prev = n->inherit;
    l->size++;
}

void lst_remove(list *l, list_node *n, bool kill_node) {
    assert(n->belong == l);
    if (l->iter == n->inherit) {
        l->iter = l->iter->next;
    }
    n->inherit->prev->next = n->inherit->next;
    n->inherit->next->prev = n->inherit->prev;
    if (kill_node) {
        lst_kill_node(n);
    }
    else {
        n->inherit->prev = NULL;
        n->inherit->next = NULL;
        n->belong = NULL;
    }
    l->size--;
}

int lst_size(list *l) {
    return l->size;
}

void lst_iter_init(list *l, bool reverse) {
    l->iter = reverse ? l->dummy->inherit->prev : l->dummy->inherit->next;
    l->reverse = reverse;
}

bool lst_iter(list *l, void *dst) {
    assert(l->iter);
    if (l->iter == l->dummy->inherit) {
        return false;
    }
    if (dst) {
        memcpy(dst, l->iter->content, l->width);
    }
    l->iter = l->reverse ? l->iter->prev : l->iter->next;
    return true;
}

void lst_kill_node(list_node *n) {
    lnd_kill(n->inherit);
    free(n);
}

void lst_kill(list *l) {
    while (l->size--) {
        list_node *n;
        memcpy(&n, (char *)l->dummy->inherit->next->content + l->width, sizeof(list_node *));
        n->inherit->next->prev = n->inherit->prev;
        n->inherit->prev->next = n->inherit->next;
        lst_kill_node(n);
    }
    lst_kill_node(l->dummy);
    free(l);
}
