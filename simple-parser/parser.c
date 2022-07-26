
#include <stdio.h>
#include <stdlib.h>

#include "dfa_state.h"
#include "token.h"
#include "node.h"

int main(int argc, char *argv[])
{
    char *script = "1 + 3 + 23 + 999";

    printf("\nparse : %s\n", script);
    token_t *head = tokenize(script);
    tokens_dump(head);
    
    printf("\nreversed : %s\n", script);
    head = tokens_reverse(head);
    tokens_dump(head);

    printf("\nnode : %s\n", script);
    match_additive_expr(head);

    return 0;
}

