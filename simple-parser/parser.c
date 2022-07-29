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
    
    printf("\nreverse : %s\n", script);
    head = tokens_reverse(head);
    tokens_dump(head);

    printf("\nnode : %s\n", script);
    match_additive_expr(head);

    puts("\n");
    script = "int age = 1 + 12;";
    head = tokenize(script);
    tokens_dump(head);
    puts("\n");
    match_int_declare(head);

    return 0;
}

void match_int_declare(token_t *head)
{
    token_t *tmp;
    /* match integer declaration statment */
    if (head->type == keyword_int && head->next != NULL) {

        token_dump(head);
        tmp = head->next;

        if (tmp->type == id && tmp->next != NULL) {
            token_dump(tmp);
            tmp = tmp->next;
            
            if (tmp->type == assignment && tmp->next != NULL) {
                token_dump(tmp);
                match_additive_expr(tmp->next);

            } else if (tmp->type == semicolon) {
                token_dump(tmp);

            } else {
                errexit("syntax error");
            }
        } else {
            errexit("syntax error");
        }

    } else {
        errexit("syntax error");
    }

}

void match_additive_expr(token_t *head)
{
    printf("mark\n");
    token_t *tmp;
    /* expr match one integer literal */
    if (head->type == int_literal 
            && (head->next == NULL || head->next->type == semicolon)) {

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

