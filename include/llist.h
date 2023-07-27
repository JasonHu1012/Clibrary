#ifndef _LLIST_H
#define _LLIST_H

#include <stdbool.h>

typedef struct llist llist;

typedef struct llist_iter llist_iter;

typedef enum {
    // represent the head way of a linked list
    LLST_HEAD,
    // represent the tail way of a linked list
    LLST_TAIL
} llist_way;

// initialize a linked list of type occupying `width` byte(s)
llist *llst_init(int width);

// kill `llst`
void llst_kill(llist *llst);

// initialize a linked list iterator of `llst`
// `way` decides the iterator to point at head or tail of `llst`
// `llst` must have at least one node
// the iterator becomes invalid when `llst` has no nodes
llist_iter *llst_it_init(llist *llst, llist_way way);

// kill `llst_it`
void llst_it_kill(llist_iter *llst_it);

// append (copy) `src` to `llst`
// `way` decides appending at head or tail of `llst`
void llst_append(llist *llst, void *src, llist_way way);

// remove head or tail of `llst`, decided by `way`, and copy it to `dst`
// `dst` can be NULL
void llst_remove(llist *llst, void *dst, llist_way way);

// return size of `llst`
int llst_size(llist *llst);

// return whether `llst` is empty
bool llst_is_empty(llist *llst);

// move `llst_it` to the next or previous node, decided by `way`
// return false if `iter` is at head or tail, otherwise true
bool llst_it_move(llist_iter *llst_it, llist_way way);

// copy the data of the node `llst_it` points at to `dst`
void llst_it_get(llist_iter *llst_it, void *dst);

// copy `src` to the data of the node `llst_it` points at
void llst_it_set(llist_iter *llst_it, void *src);

// insert a node at next or previous position, decided by `way`, of the node `llst_it` points at
// copy `src` to the node
void llst_it_insert(llist_iter *llst_it, void *src, llist_way way);

// remove the node currently pointed by `llst_it` and copy the data to `dst`
// decided by `way`, move `llst_it` to the next or previous node
// `dst` can be NULL
void llst_it_remove(llist_iter *llst_it, void *dst, llist_way way);

#endif
