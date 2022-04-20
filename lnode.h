#ifndef _LNODE_H
#define _LNODE_H

typedef struct lnode lnode;

lnode *lnd_init(void *src, int width);
void lnd_kill(lnode *n);

struct lnode {
    struct lnode *prev;
    struct lnode *next;
    void *content;
};

#endif
