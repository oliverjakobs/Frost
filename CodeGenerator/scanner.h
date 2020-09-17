#ifndef SCANNER_H
#define SCANNER_H

typedef struct
{
    const char* start;
    const char* current;
    
    int line;
} Scanner;

typedef enum
{
    // Single-character tokens.
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,

    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_COLON,
    
    TOKEN_ASTERISK,

    // One or two character tokens.
    TOKEN_MINUS, TOKEN_ARROW,

    // Literals.
    TOKEN_IDENTIFIER, TOKEN_STRING,

    // Keywords.
    TOKEN_INCLUDE,
    TOKEN_GENERATE,

    TOKEN_ERROR,
    TOKEN_EOF
} TokenType;

typedef struct
{
    
} Token;


void scanner_init(Scanner* scanner, const char* src);



#endif /* !SCANNER_H */
