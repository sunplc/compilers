
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
    minux,
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

void tokens_dump(token_t *token);
token_t *tokenize(char *script);

#endif /* _TOKEN_H_INCLUDED_ */

