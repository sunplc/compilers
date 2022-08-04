
#ifndef _NODE_H_INCLUDED_
#define _NODE_H_INCLUDED_

#include "token.h"

enum ast_node_type {
    ast_root,               /* Program root node, entrance */

    ast_int_declaration,    /* integer variable declaration statement */
    ast_expression_stmt,    /* expression statement, with semicolon */
    ast_assignment_stmt,    /* assignment statement */
    ast_additive,
    ast_multiplicative,

    ast_id,                 /* variable identifier */
    ast_int_literal,        /* integer literal */
};

#define AST_CHILDREN_MAX 8

typedef struct ast_node {
    enum ast_node_type type;
    char text[TOKEN_TEXT_MAX];
    struct ast_node *parent;
    struct ast_node *children[AST_CHILDREN_MAX];
    char child_num;
} ast_node_t;


ast_node_t *ast_node_new(enum ast_node_type t, char *text);
void ast_node_add_child(ast_node_t *parent, ast_node_t *child, int index);
ast_node_t *match_additive(token_t **p);
ast_node_t *match_int_declare(token_t *head);
void ast_dump(ast_node_t *root, int indent);


ast_node_t *parse (char *script);
ast_node_t *prog(token_t *head);

int evaluate(ast_node_t *node, int indent);

#endif /* _NODE_H_INCLUDED_ */
