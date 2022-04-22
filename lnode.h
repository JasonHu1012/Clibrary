#ifndef _LNODE_H
#define _LNODE_H

typedef struct lnode {
    struct lnode *prev;
    struct lnode *next;
    void *content;
} lnode;

// let width = 0 to create dummy node
lnode *lnd_init(void *src, int width);
void lnd_kill(lnode *n);


#endif
