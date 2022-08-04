
/*
 * token of lexical analysis results
 */

#ifndef _TOKEN_H_INCLUDED_
#define _TOKEN_H_INCLUDED_

enum token_type {
    keyword_int,

    int_literal,

    id,

    plus,
    minus,
    star,
    slash,
    assignment,

    left_paren,
    right_paren,

    semicolon,
};


/* max token string length */
#define TOKEN_TEXT_MAX 256

typedef struct token {
    enum token_type type;
    char text[TOKEN_TEXT_MAX];
    struct token * next;
} token_t;

void errexit(char *msg);
void token_dump(token_t *token);
void tokens_dump(token_t *token);
token_t *tokens_reverse(token_t *head);
token_t *tokenize(char *script);

#endif /* _TOKEN_H_INCLUDED_ */

