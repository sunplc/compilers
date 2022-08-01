
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "ast_node.h"

ast_node_t *match_multiplicative(token_t **p);
ast_node_t *match_primary(token_t **p);


ast_node_t *ast_node_new(enum ast_node_type t, char *text)
{
    ast_node_t *node = malloc(sizeof(ast_node_t));
    node->type = t;
    size_t len = strlen(text);
    if (len >= TOKEN_TEXT_MAX) {
        strncpy(node->text, text, TOKEN_TEXT_MAX - 1);
        node->text[TOKEN_TEXT_MAX - 1] = '\0';
    } else {
        strcpy(node->text, text);
    }
    node->child_num = 0;
    return node;
}

void ast_node_add_child(ast_node_t *parent, ast_node_t *child, int index)
{
    parent->children[index] = child;
    ++parent->child_num;
}

/* 解析整形变量声明语句 */
ast_node_t *match_int_declare(token_t *head)
{
    ast_node_t *node = NULL;
    token_t *tmp;
    /* match integer declaration statment */
    if (head->type == keyword_int && head->next != NULL) {

        tmp = head->next;

        if (tmp->type == id && tmp->next != NULL) {
            node = ast_node_new(ast_int_declaration, tmp->text);
            tmp = tmp->next;
            
            if (tmp->type == assignment && tmp->next != NULL) {
                ast_node_t *child = match_additive(&tmp->next);
                if (child != NULL) {
                    ast_node_add_child(node, child, 0);
                }

            } else if (tmp->type == semicolon) {

            } else {
                errexit("syntax error");
            }
        } else {
            errexit("syntax error");
        }

    } else {
        errexit("syntax error");
    }

    return node;
}

ast_node_t *match_additive(token_t **p)
{
    ast_node_t *child1 = match_multiplicative(p);
    ast_node_t *node = child1;

    token_t *head = *p;
    if (child1 == NULL || head == NULL) {
        return node;
    }

    /* expr match one integer literal */
    if (head->type == plus || head->type == minus) {
        node = ast_node_new(ast_additive, head->text);
        head = head->next;

        ast_node_t *child2 = match_additive(&head);
        *p = head;

        if (child2 != NULL) {
            ast_node_add_child(node, child1, 0);
            ast_node_add_child(node, child2, 1);
        } else {
            errexit("invalid additive expression, expecting right part.");
        }
    }

    return node;
}

ast_node_t *match_multiplicative(token_t **p)
{
    ast_node_t *child1 = match_primary(p);
    ast_node_t *node = child1;

    token_t *head = *p;
    if (child1 == NULL || head == NULL) {
        return node;
    }

    if (head->type == star || head->type == slash) {
        node = ast_node_new(ast_multiplicative, head->text);
        head = head->next;

        ast_node_t *child2 = match_multiplicative(&head);
        *p = head;

        if (child2 != NULL) {
            ast_node_add_child(node, child1, 0);
            ast_node_add_child(node, child2, 1);
        } else {
            errexit("invalid multiplicative expression, expecting right part.");
        }
    }

    return node;
}

ast_node_t *match_primary(token_t **p)
{
    token_t *head = *p;
    ast_node_t *node = NULL;

    if (head == NULL)
        return node;

    if (head->type == int_literal) {
        node = ast_node_new(ast_int_literal, head->text);
        *p = head->next;
    } else if (head->type == id) {
        node = ast_node_new(ast_id, head->text);
        *p = head->next;
    } else if (head->type == left_paren) {
        head = head->next;
        node = match_additive(&head);
        if (node != NULL) {
            if (head != NULL && head->type == right_paren) {
                head = head->next;
                *p = head;
            } else {
                errexit("expecting right parenthesis");
            }
        } else {
            errexit("expecting an additive expression inside parenthesis.");
        }
    }
    return node;
}

void ast_dump(ast_node_t *root, int indent)
{
    int i = indent;
    while (i--) {
        putchar('\t');
    }

    printf("%s\n", root->text);

    int num;
    for (num = 0; num < root->child_num; num++) {
        ast_dump(root->children[num], indent + 1);
    }
}

ast_node_t *parse (char *script)
{
    token_t *token = tokenize(script);
    ast_node_t *root = prog(token);
    return root;
}

ast_node_t *prog(token_t *head)
{
    ast_node_t *root = ast_node_new(ast_root, "");
    ast_node_t *child = match_additive(&head);
    if (child != NULL)
        ast_node_add_child(root, child, 0);
    return root;
}

int evaluate(ast_node_t *node, int indent)
{
    int i = indent;
    while (i--) {
        putchar('\t');
    }

    int result = 0;
    printf("Calculating: %s\n", node->text);


    if (node->type == ast_root) {
        for (i = 0; i < node->child_num; i++) {
            result = evaluate(node->children[i], indent + 1);
        }
    } else if (node->type == ast_additive) {
        int op1 = evaluate(node->children[0], indent + 1);
        int op2 = evaluate(node->children[1], indent + 1);
        if (strcmp(node->text, "+") == 0) {
            result = op1 + op2;
        } else {
            result = op1 - op2;
        }
    } else if (node->type == ast_multiplicative) {
        int op1 = evaluate(node->children[0], indent + 1);
        int op2 = evaluate(node->children[1], indent + 1);
        if (strcmp(node->text, "*") == 0) {
            result = op1 * op2;
        } else {
            result = op1 / op2;
        }
    } else if (node->type == ast_int_literal) {
        result = atoi(node->text);
    }

    i = indent;
    while (i--) {
        putchar('\t');
    }

    printf("Result: %d\n", result);

    return result;
}
