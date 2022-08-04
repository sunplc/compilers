#include <stdio.h>
#include <stdlib.h>

#include "dfa_state.h"
#include "token.h"
#include "ast_node.h"

int main(int argc, char *argv[])
{
    char *script = "1+2*3+4-5";

    printf("\nscript : %s\n", script);
    ast_node_t *root = parse(script);
    ast_dump(root, 0);
    int result = evaluate(root, 0);
    printf("Result: %d\n", result);


    script = "int age = 1 + 12;";
    printf("\nscript : %s\n", script);
    token_t *head = tokenize(script);
    root = match_int_declare(head);
    ast_dump(root, 0);

    return 0;
}

