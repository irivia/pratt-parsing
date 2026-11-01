#pragma once

#include "list.h"
#include <stdbool.h>
#include "log.h"

#define TOKEN_LIST() \
    X(TOKEN_NONE), \
    X(TOKEN_NUM), \
    X(TOKEN_STRING), \
    X(TOKEN_PLUS), \
    X(TOKEN_MINUS), \
    X(TOKEN_STAR), \
    X(TOKEN_SLASH), \
    X(TOKEN_CARET), \
    X(TOKEN_LEFT_PAREN), \
    X(TOKEN_RIGHT_PAREN), \
    X(TOKEN_COMMA), \
    X(TOKEN_EQUAL), \
    X(TOKEN_DOLLAR), \
    X(TOKEN_ANS), \
    X(TOKEN_IDENTIFIER), \
    X(TOKEN_LET), \
    X(TOKEN_EXIT), \
    X(TOKEN_OR), \
    X(TOKEN_AND), \
    X(TOKEN_EQEQ), \
    X(TOKEN_NOT), \
    X(TOKEN_NOTEQ), \
    X(TOKEN_LESS), \
    X(TOKEN_LESSEQ), \
    X(TOKEN_GREATER), \
    X(TOKEN_GREATEREQ), \
    X(TOKEN_TRUE), \
    X(TOKEN_FALSE), \
    X(TOKEN_QMARK), \
    X(TOKEN_COLON), \
    X(TOKEN_END), \
    X(TOKEN_ERROR), \
    X(TOKEN_COUNT),

#define X(t) t

typedef enum {
    TOKEN_LIST()
} TokenType;

#undef X

#define X(t) #t

static const char* const TOKEN_NAMES[] = {
    TOKEN_LIST()
};

#undef X

typedef struct {
    TokenType type;
    const char *start;
    int len;
} Token;

typedef struct {
    const char *text;
    int current;
    bool error;
    LoggingInfo logging;
} Lexer;

LIST_DEF(TokenList, Token);

Lexer lexer_new(const char *text);
bool tokenize(const char *text, TokenList *output);
void print_tokenlist(TokenList *list);

