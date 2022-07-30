#ifndef _FUNC_H
#define _FUNC_H

#include <stdbool.h>

// smart malloc
void *salloc(int width, int *len, int dim);
int randi(int min, int max);
double randf(double min, double max);
bool str_to_num(char *str, int *n);
int *distinct_rand(int start, int end, int count);

#endif
