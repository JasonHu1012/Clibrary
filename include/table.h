#ifndef _TABLE_H
#define _TABLE_H

#include <stdbool.h>

typedef struct table table;

// initialize a hash table
// key type is string
// value type occupies `width` byte(s)
table *tbl_init(int width);

// kill `tbl`
void tbl_kill(table *tbl);

// copy `tbl[key]` to `dst`
// `dst` won't be modified if `key` isn't found
void tbl_get(table *tbl, char *key, void *dst);

// copy `src` to `tbl[key]`
void tbl_set(table *tbl, char *key, void *src);

// return whether `key` is in `tbl`
bool tbl_contain(table *tbl, char *key);

// remove `key` from `tbl`
// `key` must be in `tbl`
void tbl_remove(table *tbl, char *key);

// return size of `tbl`
int tbl_size(table *tbl);

// return whether `tbl` is empty
bool tbl_is_empty(table *tbl);

// return an array of strings containing keys of `tbl`
// the array length can be retrieved by `tbl_size`
// users should `free` the array and the strings
char **tbl_keys(table *tbl);

#endif
