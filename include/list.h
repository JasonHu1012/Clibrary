#ifndef _LIST_H
#define _LIST_H

#include <stdbool.h>

typedef struct list list;

// initialize a list of type occupying `width` byte(s)
// initial size is 0
list *lst_init(int width);

// kill `lst`
void lst_kill(void *lst);

// kill `lst` with callback function
// before killing `lst`, will call `kill()` to kill each element
void lst_kill_f(list *lst, void (*kill)(void *ptr));

// set the size of `lst` to `size`
void lst_set_size(list *lst, int size);

// return size of `lst`
int lst_size(list *lst);

// return whether `lst` is empty
bool lst_is_empty(list *lst);

// copy the `index`-th element of `lst` to `dst`
void lst_get(list *lst, int index, void *dst);

// copy `src` to the `index`-th element of `lst`
void lst_set(list *lst, int index, void *src);

// append (copy) `src` at the end of `lst`
void lst_append(list *lst, void *src);

// remove the last element of `lst`
// `dst` can be NULL
void lst_pop(list *lst, void *dst);

// insert (copy) `src` into the `index`-th element of `lst`
void lst_insert(list *lst, int index, void *src);

// remove the `index`-th element of `lst` and copy it to `dst`
// `dst` can be NULL
void lst_remove(list *lst, int index, void *dst);

// return whether `lst` contains `target`
bool lst_contain(list *lst, void *target);

// return the number of `target` in `lst`
int lst_count(list *lst, void *target);

#endif
