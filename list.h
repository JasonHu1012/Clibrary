#ifndef _LIST_H
#define _LIST_H

#include "lnode.h"
#include <stdbool.h>

typedef struct list list;
typedef struct list_node list_node;

list *lst_init(int width);
list_node *lst_init_node(void *src, int width);
// append either data or node; the other should be NULL
// if node, the data won't be copied into the list; user should keep the data
void lst_append(list *l, void *src, list_node *n);
void lst_remove(list *l, list_node *n, bool kill_node);
int lst_size(list *l);
void lst_iter_init(list *l, bool reverse);
// return false when reaching end; should call lst_iter_init first
bool lst_iter(list *l, void *dst);
void lst_kill_node(list_node *n);
void lst_kill(list *l);

struct list {
    list_node *dummy;
    lnode *iter;
    int size;
    int width;
    bool reverse;
};

struct list_node {
    lnode *inherit;
    int width;
    list *belong;
};

#endif
