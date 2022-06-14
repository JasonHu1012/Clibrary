#ifndef _LIST_H
#define _LIST_H

#include "lnode.h"
#include <stdbool.h>

typedef struct list list;
typedef struct list_node list_node;
typedef struct list_iter list_iter;

list *lst_init(int width);
list_node *lst_init_node(void *src, int width);
list_iter *lst_init_iter(list *l, bool reverse);
// add either data or node; the other should be NULL
// if node, the node won't be copied into the list, and user should keep it
void lst_add_head(list *l, void *src, list_node *n);
void lst_add_tail(list *l, void *src, list_node *n);
void lst_add_iter_next(list_iter *i, void *src, list_node *n);
void lst_add_iter_prev(list_iter *i, void *src, list_node *n);
// remove the node from list
void lst_remove(list_node *n);
void lst_iter_remove(list_iter *i);
int lst_size(list *l);
// return false when reaching end
bool lst_iter_next(list_iter *i, void *dst);
void lst_iter_reset(list_iter *i);
void lst_kill_iter(list_iter *i);
// get and set node's content
void lst_get_node(list_node *n, void *dst, int width);
void lst_set_node(list_node *n, void *src, int width);
// if the node is still in the list, it won't be removed
// this function only indicates that the node should be killed when removed
void lst_kill_node(list_node *n);
// lst_kill_iter can't be called after lst_kill
// node created by user won't be killed
void lst_kill(list *l);

#endif
