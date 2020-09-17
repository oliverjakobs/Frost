#include "scanner.h"

#include <stdio.h>
#include <string.h>

static inline int is_end_of_line(char c)
{
    return (c == '\n') || (c == '\r');
}

static inline int is_whitespace(char c)
{
    return (c == ' ') || (c == '\t') || is_end_of_line(c);
}

static inline int is_alpha(char c)
{
    return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'));
}

static inline int is_number(char c)
{
    return (c >= '0') && (c <= '9');
}

static char* skip_whitespaces(char* cursor)
{
    while (1)
    {
        if (is_whitespace(cursor[0]))
        {
            cursor++;
        }
        else if (cursor[0] == '#') /* Comments */
        {
            while (cursor[0] && !is_end_of_line(cursor[0]))
                cursor++;
        }
        else
        {
            return cursor;
        }
    }
}

size_t token_array_len(Token arr)
{
    if (arr.type != TOKEN_ARRAY) return 0;

    int count = 1;
    for (size_t i = 0; i < arr.len; ++i)
    {
        if (arr.text[i] == ',')
            count++;
    }

    return count;
}

Token token_array_at(Token arr, int index)
{
    Token token;
    token.text = arr.text;
    token.type = TOKEN_ERROR;
    token.len = arr.len;

    size_t arr_len = token_array_len(arr);

    if (arr.type != TOKEN_ARRAY || index >= arr_len) return token;

    char* cursor = skip_whitespaces(++arr.text);
    for (int i = 0; i <= index; ++i)
    {
        token.text = cursor;

        if (i == arr_len - 1)
            cursor = strpbrk(token.text, " ]");
        else
            cursor = memchr(token.text, ',', arr.len);

        if (!cursor) return token;

        token.len = cursor - token.text;
        cursor = skip_whitespaces(++cursor);
    }

    token.type = TOKEN_ARRAY_ELEMENT;
    return token;
}

int token_cmp(Token token, const char* str)
{
    return strncmp(token.text, str, token.len);
}

Token scanner_get_next(Scanner* scanner)
{
    scanner->cursor = skip_whitespaces(scanner->cursor);

    Token token;
    token.type = TOKEN_ERROR;
    token.text = scanner->cursor;
    token.len = 1;

    char c = scanner->cursor[0];
    switch (c)
    {
    case '\0': token.type = TOKEN_END_OF_STREAM; scanner->cursor++; break;

    case ':': token.type = TOKEN_COLON; scanner->cursor++; break;
    case ',': token.type = TOKEN_COMMA; scanner->cursor++; break;
    case '*': token.type = TOKEN_ASTERISK; scanner->cursor++; break;

    case '(': token.type = TOKEN_OPEN_PAREN; scanner->cursor++; break;
    case ')': token.type = TOKEN_CLOSE_PAREN; scanner->cursor++; break;

    case '{': 
        token.type = TOKEN_OPEN_BLOCK;

        char* reset = ++scanner->cursor;

        /* count tokens till first TOKEN_CLOSE_BLOCK */
        Token next = scanner_get_next(scanner);
        token.len = 0;
        while (next.type != TOKEN_CLOSE_BLOCK)
        {
            if (token.type == TOKEN_END_OF_STREAM)
                break;

            token.len++;

            next = scanner_get_next(scanner);
        }
        scanner->cursor = reset;

        break;

    case '}': token.type = TOKEN_CLOSE_BLOCK; scanner->cursor++; break;

    case '-':
        scanner->cursor++;
        if (scanner->cursor[0] == '>')
        {
            token.type = TOKEN_ARROW;
            token.len = 2;
            ++scanner->cursor;
        }
        break;

    case '[':
        token.type = TOKEN_ARRAY;
        token.text = scanner->cursor;

        while (scanner->cursor[0] && scanner->cursor[0] != ']')
            ++scanner->cursor;
        
        if (scanner->cursor[0] == ']')
            ++scanner->cursor;

        token.len = scanner->cursor - token.text;
        break;

    case '"':
        scanner->cursor++;
        token.type = TOKEN_STRING;
        token.text = scanner->cursor;

        while (scanner->cursor[0] && scanner->cursor[0] != '"')
        {
            if (scanner->cursor[0] == '\\' && scanner->cursor[1])
                ++scanner->cursor;
            ++scanner->cursor;
        }
        
        token.len = scanner->cursor - token.text;

        if (scanner->cursor[0] == '"')
            ++scanner->cursor;

        break;
    default:
        scanner->cursor++;
        if (is_alpha(c))
        {
            token.type = TOKEN_IDENTIFIER;

            while (is_alpha(scanner->cursor[0]) 
                || is_number(scanner->cursor[0]) 
                || (scanner->cursor[0] == '_'))
                ++scanner->cursor;
            
            token.len = scanner->cursor - token.text;
        }
        break;
    }

    return token;
}

Token scanner_skip_till(Scanner* scanner, TokenType type)
{
    Token token = scanner_get_next(scanner);
    while (token.type != type)
    {
        if (token.type == TOKEN_END_OF_STREAM)
            break;

        token = scanner_get_next(scanner);
    }

    return token;
}

int scanner_require(Scanner* scanner, TokenType type)
{
    return scanner_get_next(scanner).type == type;
}

void print_token(Token token)
{
    if (token.type == TOKEN_OPEN_BLOCK)
        printf("%2d: {}: %d\n", token.type, token.len);
    else
        printf("%2d: %.*s\n", token.type, token.len, token.text, token.len);
}