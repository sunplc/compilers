
/* lexical analysis source code, produce tokens */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#include "dfa_state.h"
#include "token.h"

static const char *token_type_names[] = {
    "keyword_int",

    "int_literal",

    "id",

    "plus",
    "minux",
    "star",
    "slash",
    "assignment",

    "left_paren",
    "right_paren",

    "semicolon",
};

void errexit(char *msg)
{
    fprintf(stderr, "ERROR: %s\n", msg);
    exit(1);
}


static token_t* token_new(const char *text, enum token_type type)
{
    token_t *new = malloc(sizeof(token_t));
    new->type = type;
    strcpy(new->text, text);
    return new;
}

void token_dump(token_t *head)
{
    if (head != NULL)
        printf("%s\t\t%s\n", head->text, token_type_names[head->type]);
}

void tokens_dump(token_t *head)
{
    printf("text:\t\ttype:\n");

    while (head != NULL) {
        printf("%s\t\t%s\n", head->text, token_type_names[head->type]);
        head = head->next;
    }
}

/* Reverse single direction linked list */
token_t *tokens_reverse(token_t *head)
{
    token_t *prev = NULL, *next = NULL;
    /* printf(">>> %p %p %p\n", head, prev, next); */

    while (head != NULL) {
        next = head->next;

        if (prev != NULL)
            head->next = prev;
        else
            head->next = NULL;

        prev = head;
        head = next;
    }

    return prev;
}

/* append character to token's text, check string boundry */
static void append_char(char *text, char c)
{
    size_t token_text_len = strlen(text);

    assert(token_text_len + 1 < TOKEN_TEXT_MAX);

    text[token_text_len] = c;
    text[token_text_len + 1] = '\0';
}

/* the head and tail of linked list of tokens */
token_t *head, *tail;

/* wether get next character from script */
int get_next_char = 1;

/* deterministic finite automation's state */
enum dfa_state state = state_initial;

/* save token and clear new's state */
static void save_token(token_t *new)
{
    get_next_char = 0;
    state = state_initial;

    if (strlen(new->text)) {
        /* create an new token */
        token_t *tmp = token_new(new->text, new->type);

        if (head == NULL) {
            head = tmp;
            tail = tmp;
        } else {
            tail->next = tmp;
            tail = tmp;
        }
    }


    /* init original token */
    new->type = state_initial;
    new->text[0] = '\0';
}


token_t *tokenize(char *script) {
    head = NULL;
    get_next_char = 1;
    char c;

    char *cursor = script;
    token_t *t = token_new("", state);

    while (1) {
        if (get_next_char) {
            c = *(cursor++);
            if (!c) {
                save_token(t);
                break;
            }
            /* printf(">>> char: %c\t(integer %d)\n", c, c); */
        }
        get_next_char = 1;


        switch(state) {
        case state_initial:
            if (isdigit(c)) {
                state = state_int_literal;
                t->type = int_literal;
                append_char(t->text, c);
            } else if (c == 'i') {
                state = state_keyword_int1;
                t->type = keyword_int;
                append_char(t->text, c);
            } else if (c == '+') {
                state = state_plus;
                t->type = plus;
                append_char(t->text, c);
            } else if (c == '-') {
                state = state_minus;
                t->type = minus;
                append_char(t->text, c);
            } else if (c == '*') {
                state = state_star;
                t->type = star;
                append_char(t->text, c);
            } else if (c == '/') {
                state = state_slash;
                t->type = slash;
                append_char(t->text, c);
            } else if (c == '=') {
                state = state_assignment;
                t->type = assignment;
                append_char(t->text, c);
            } else if (c == '(') {
                state = state_left_paren;
                t->type = left_paren;
                append_char(t->text, c);
            } else if (c == ')') {
                state = state_right_paren;
                t->type = right_paren;
                append_char(t->text, c);
            } else if (c == ';') {
                state = state_semicolon;
                t->type = semicolon;
                append_char(t->text, c);
            } else if (isalnum(c)) {
                state = state_id;
                t->type = id;
                append_char(t->text, c);
            } else if (isspace(c)) {

            } else {
                fprintf(stderr, "lexical analysis error, "
                        "invalid character: '%c' (integer %d)\n", c, c);
                exit(1);
            }

            break;

        case state_keyword_int1:
            if (c == 'n') {
                state = state_keyword_int2;
                append_char(t->text, c);
            } else if (isalnum(c)) {
                state = state_id;
                t->type = id;
                append_char(t->text, c);
            } else {
                save_token(t);
            }
            break;

        case state_keyword_int2:
            if (c == 't') {
                state = state_keyword_int3;
                append_char(t->text, c);
            } else if (isalnum(c)) {
                state = state_id;
                t->type = id;
                append_char(t->text, c);
            } else {
                save_token(t);
            }
            break;

        case state_keyword_int3:
            if (isalnum(c)) {
                state = state_id;
                t->type = id;
                append_char(t->text, c);
            } else {
                save_token(t);
            }
            break;
        
        case state_id:
            if (isalnum(c)) {
                append_char(t->text, c);
            } else {
                save_token(t);
            }
            break;

        case state_int_literal:
            if (isdigit(c)) {
                append_char(t->text, c);
            } else {
                save_token(t);
            }
            break;

        case state_plus:
        case state_minus:
        case state_star:
        case state_slash:
        case state_assignment:
        case state_left_paren:
        case state_right_paren:
        case state_semicolon:
            save_token(t);
            break;
        }
    }

    return head;
}


