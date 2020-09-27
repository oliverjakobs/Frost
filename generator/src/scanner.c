#include "scanner.h"

#include <stdio.h>
#include <string.h>

static int is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

static int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static Token make_token(const Scanner* scanner, TokenType type)
{
    Token token;
    token.type = type;
    token.start = scanner->start;
    token.len = scanner->current - scanner->start;
    token.line = scanner->line;

    return token;
}

static Token make_token_error(const Scanner* scanner, const char* message)
{
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.len = strlen(message);
    token.line = scanner->line;

    return token;
}

static Token make_string(Scanner* scanner)
{
    while(scanner->current[0] != '"')
    {
        if (scanner->current[0] == '\0' || scanner->current[0] == '\n')
            return make_token_error(scanner, "Unterminated string.");

        scanner->current++;
    }

    scanner->current++; /* closing quote */
    return make_token(scanner, TOKEN_STRING);
}

static TokenType check_keyword(const Scanner* scanner, const char* keyword, TokenType type)
{
    size_t len = strlen(keyword);

    if ((scanner->current - scanner->start == len) && memcmp(scanner->start, keyword, len) == 0)
        return type;

    return TOKEN_IDENTIFIER;
}

static TokenType get_identifier_type(const Scanner* scanner)
{
    switch (scanner->start[0])
    {
    case 'd': return check_keyword(scanner, "define", TOKEN_DEFINE);
    case 'e': return check_keyword(scanner, "enum", TOKEN_ENUM);
    case 'g': return check_keyword(scanner, "generate", TOKEN_GENERATE);
    case 'i': return check_keyword(scanner, "include", TOKEN_INCLUDE);
    }

    return TOKEN_IDENTIFIER;
}

static Token make_identifier(Scanner* scanner)
{
    while (is_alpha(scanner->current[0]) || is_digit(scanner->current[0])) 
        scanner->current++;

    return make_token(scanner, get_identifier_type(scanner));
}

void scanner_init(Scanner* scanner, const char* source)
{
    scanner->start = source;
    scanner->current = source;

    scanner->line = 1;
}

static int scanner_match(Scanner* scanner, char c)
{
    if (!(scanner->current[0] && scanner->current[0] == c))
        return 0;

    scanner->current++;
    return 1;
}

static void scanner_skip_whitespaces(Scanner* scanner)
{
    while(1)
    {
        switch (scanner->current[0])
        {
        case '\n':
            scanner->line++;
        case ' ':
        case '\r':
        case '\t':
            scanner->current++;
            break;

        /* Comments */
        case '#':
            while(scanner->current[0] && scanner->current[0] != '\n')
                scanner->current++;
            break;

        default:
            return;
        }
    }
}

Token scanner_get_next(Scanner* scanner)
{
    scanner_skip_whitespaces(scanner);
    
    scanner->start = scanner->current;

    if (!scanner->current[0]) return make_token(scanner, TOKEN_EOF);

    char c = scanner->current[0];
    scanner->current++;

    if (is_alpha(c)) return make_identifier(scanner);

    switch (c)
    {
    case ':': return make_token(scanner, TOKEN_COLON);
    case ',': return make_token(scanner, TOKEN_COMMA);
    case '*': return make_token(scanner, TOKEN_ASTERISK);

    case '(': return make_token(scanner, TOKEN_LEFT_PAREN);
    case ')': return make_token(scanner, TOKEN_RIGHT_PAREN);
    case '{': return make_token(scanner, TOKEN_LEFT_BRACE);
    case '}': return make_token(scanner, TOKEN_RIGHT_BRACE);
    case '[': return make_token(scanner, TOKEN_LEFT_BRACKET);
    case ']': return make_token(scanner, TOKEN_RIGHT_BRACKET);

    case '-':
        if (scanner_match(scanner, '>'))
            return make_token(scanner, TOKEN_ARROW);
        else
            return make_token_error(scanner, "Expected '>' after '-'.");

    case '"': return make_string(scanner);
    
    default:
        break;
    }

    return make_token_error(scanner, "Unexpected character.");
}

int token_cmp(Token* token, const char* str)
{
    return strncmp(token->start, str, token->len);
}

void print_token(Token* token)
{
    printf("%2d: %.*s\n", token->type, token->len, token->start);
}