
#include <stdio.h>

#include "token.h"


void match_additive_expr(token_t *head)
{
    token_t *tmp;
    /* expr match one integer literal */
    if (head->type == int_literal && head->next == NULL) {

        token_dump(head);

    } else if (head->type == int_literal && head->next != NULL) {
        tmp = head->next;
        if (tmp->type == plus && tmp->next != NULL) {
            match_additive_expr(tmp->next);
            token_dump(head);
            token_dump(tmp);
        } else {
            errexit("syntax error");
        }

    } else {
        errexit("syntax error");
    }
}
