
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

static token_t* token_new(const char *text, enum token_type type)
{
    token_t *new = malloc(sizeof(token_t));
    new->type = type;
    strcpy(new->text, text);
    return new;
}

static void token_add(token_t *tail, const char *text, enum token_type type)
{
    token_t *new = token_new(text, type);
    tail->next = new;
}

void tokens_dump(token_t *head)
{
    printf("text:\t\ttype:\n");

    while (head != NULL) {
        printf("%s\t\t%s\n", head->text, token_type_names[head->type]);
        head = head->next;
    }
}

/* append character to token's text, check string boundry */
void append_char(char *text, char c)
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

void save_token(token_t *new)
{
    get_next_char = 0;
    state = state_initial;

    if (strlen(new->text)) {
        /* create an new token */
        token_t *tmp = malloc(sizeof(token_t));

        tmp->type = new->type;
        strcpy(tmp->text, new->text);

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
    char *cursor = script;
    token_t *t = token_new("", state);
    char c;

    while (1) {
        if (get_next_char) {
            c = *(cursor++);
            if (!c) {
                save_token(t);
                break;
            }
        }
        get_next_char = 1;

        /* printf(">>> Char: %d %c\n", c, c); */

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
                t->type = minux;
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

