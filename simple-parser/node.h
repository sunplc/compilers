
#ifndef _NODE_H_INCLUDED_
#define _NODE_H_INCLUDED_

typedef struct node {
    int val;
    struct node * left;
    struct node * right;
} node_t;

void match_additive_expr(token_t *head);

#endif /* _NODE_H_INCLUDED_ */
