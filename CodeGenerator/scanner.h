#ifndef SCANNER_H
#define SCANNER_H

#include <stdlib.h>

typedef enum
{
    TOKEN_IDENTIFIER,
    TOKEN_STRING,

    TOKEN_ARRAY,
    TOKEN_ARRAY_ELEMENT,

    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_ASTERISK,

    TOKEN_ARROW,

    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
    TOKEN_OPEN_BLOCK,
    TOKEN_CLOSE_BLOCK,

    TOKEN_ERROR,
    TOKEN_END_OF_STREAM
} TokenType;

typedef struct
{
    TokenType type;

    size_t len;
    char* text;
} Token;

size_t token_array_len(Token arr);
Token token_array_at(Token arr, int index);

int token_cmp(Token token, const char* str);

typedef struct
{
    char* cursor;
} Scanner;

Token scanner_get_next(Scanner* scanner);
Token scanner_skip_till(Scanner* scanner, TokenType type);

int scanner_require(Scanner* scanner, TokenType type);

void print_token(Token token);

#endif /* !SCANNER_H */
