#include "func.h"
#include <stdlib.h>
#include <assert.h>

void *salloc(int width, int *len, int dim) {
    assert(dim > 0 && width > 0);
    int total_len = 0;
    int temp = len[0];
    for (int i = 1; i < dim; i++) {
        total_len += sizeof(void *) * temp;
        temp *= len[i];
    }
    total_len += temp * width;
    void *output = malloc(total_len);
    if (dim == 1) {
        return output;
    }
    void **cur = (void **)output;
    char *target = (char *)&cur[len[0]];
    temp = len[0];
    for (int i = 1; i < dim - 1; i++) {
        for (int j = 0; j < temp; j++) {
            cur[j] = (void *)&target[sizeof(void *) * j * len[i]];
        }
        cur += temp;
        temp *= len[i];
        target += sizeof(void *) * temp;
    }
    for (int i = 0; i < temp; i++) {
        cur[i] = (void *)&target[width * i * len[dim - 1]];
    }
    return output;
}
