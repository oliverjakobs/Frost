#ifndef SCANNER_H
#define SCANNER_H

#include <stdlib.h>

#include "tb_array.h"

typedef enum
{
    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_ASTERISK,

    TOKEN_ARROW,

    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    
    /* literals */
    TOKEN_IDENTIFIER,
    TOKEN_STRING,

    /* keywords */
    TOKEN_ENUM,
    TOKEN_DEFINE,
    TOKEN_INCLUDE,
    TOKEN_GENERATE,

    TOKEN_ERROR,
    TOKEN_EOF
} TokenType;

typedef struct
{
    TokenType type;

    const char* start;
    size_t len;

    int line;
} Token;

typedef struct
{
    const char* start;
    const char* current;
    int line;
} Scanner;

void scanner_init(Scanner* scanner, const char* source);

Token scanner_get_next(Scanner* scanner);
tb_array scanner_get_tokens(Scanner* scanner, size_t* count_ptr);

int token_cmp(Token* token, const char* str);

void print_token(Token* token);

#endif /* !SCANNER_H */
