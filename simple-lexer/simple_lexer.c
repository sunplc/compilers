
#include <stdio.h>
#include <stdlib.h>

#include "dfa_state.h"
#include "token.h"

int main(int argc, char *argv[])
{
    char *script = "a +b+123 * 12;";

    printf("\nparse : %s\n", script);
    token_t *head = tokenize(script);
    tokens_dump(head);

    script = "int age >= 45;";

    printf("\nparse : %s\n", script);
    head = tokenize(script);
    tokens_dump(head);

    return 0;
}

