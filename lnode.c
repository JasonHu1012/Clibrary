#include "lnode.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

lnode *lnd_init(void *src, int width) {
    assert(width >= 0);
    lnode *output = (lnode *)calloc(1, sizeof(lnode));
    if (width) {
        assert(src);
        output->content = malloc(width);
        memcpy(output->content, src, width);
    }
    return output;
}

void lnd_kill(lnode *n) {
    if (n->content) {
        free(n->content);
    }
    free(n);
}
