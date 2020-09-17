#include <stdlib.h>

#define TB_FILE_IMPLEMENTATION
#include "tb_file.h"

#include <string.h>

typedef struct
{
    char* cursor;
} Tokenizer;

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

static inline int token_cmp(Token token, const char* str)
{
    return strncmp(token.text, str, token.len);
}

static void token_print(Token token)
{
    if (token.type == TOKEN_OPEN_BLOCK)
        printf("%2d: {}: %d\n", token.type, token.len);
    else
        printf("%2d: %.*s\n", token.type, token.len, token.text, token.len);
}

static inline int is_end_of_line(char c)
{
    return (c == '\n') || (c == '\r');
}

static inline int is_whitespace(char c)
{
    return (c == ' ') || (c == '\t') || is_end_of_line(c);
}

static char* skip_whitespaces(char* cursor)
{
    while (1)
    {
        if (is_whitespace(cursor[0]))
        {
            ++cursor;
        }
        else if (cursor[0] == '#') /* Comments */
        {
            cursor++;
            while (cursor[0] && !is_end_of_line(cursor[0]))
                cursor++;
        }
        else
        {
            return cursor;
        }
    }
}

static inline int is_alpha(char c)
{
    return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'));
}

static inline int is_number(char c)
{
    return (c >= '0') && (c <= '9');
}

static int is_identifier_symbol(char c)
{
    return is_alpha(c) || is_number(c) || (c == '_');
}

static Token get_token(Tokenizer* tokenizer)
{
    tokenizer->cursor = skip_whitespaces(tokenizer->cursor);

    Token token;
    token.type = TOKEN_ERROR;
    token.text = tokenizer->cursor;
    token.len = 1;

    char c = tokenizer->cursor[0];
    switch (c)
    {
    case '\0': token.type = TOKEN_END_OF_STREAM; tokenizer->cursor++; break;

    case ':': token.type = TOKEN_COLON; tokenizer->cursor++; break;
    case ',': token.type = TOKEN_COMMA; tokenizer->cursor++; break;
    case '*': token.type = TOKEN_ASTERISK; tokenizer->cursor++; break;

    case '(': token.type = TOKEN_OPEN_PAREN; tokenizer->cursor++; break;
    case ')': token.type = TOKEN_CLOSE_PAREN; tokenizer->cursor++; break;

    case '{': 
        token.type = TOKEN_OPEN_BLOCK;

        char* reset = ++tokenizer->cursor;
        /* count tokens till first TOKEN_CLOSE_BLOCK */
        Token next = get_token(tokenizer);
        token.len = 0;
        while (next.type != TOKEN_CLOSE_BLOCK)
        {
            if (token.type == TOKEN_END_OF_STREAM)
                break;

            token.len++;

            next = get_token(tokenizer);
        }
        tokenizer->cursor = reset;

        break;

    case '}': token.type = TOKEN_CLOSE_BLOCK; tokenizer->cursor++; break;

    case '-':
        tokenizer->cursor++;
        if (tokenizer->cursor[0] == '>')
        {
            token.type = TOKEN_ARROW;
            token.len = 2;
            ++tokenizer->cursor;
        }
        break;

    case '[':
        token.type = TOKEN_ARRAY;
        token.text = tokenizer->cursor;

        while (tokenizer->cursor[0] && tokenizer->cursor[0] != ']')
            ++tokenizer->cursor;
        
        if (tokenizer->cursor[0] == ']')
            ++tokenizer->cursor;

        token.len = tokenizer->cursor - token.text;
        break;

    case '"':
        tokenizer->cursor++;
        token.type = TOKEN_STRING;
        token.text = tokenizer->cursor;

        while (tokenizer->cursor[0] && tokenizer->cursor[0] != '"')
        {
            if (tokenizer->cursor[0] == '\\' && tokenizer->cursor[1])
                ++tokenizer->cursor;
            ++tokenizer->cursor;
        }
        
        token.len = tokenizer->cursor - token.text;

        if (tokenizer->cursor[0] == '"')
            ++tokenizer->cursor;

        break;
    default:
        tokenizer->cursor++;
        if (is_alpha(c))
        {
            token.type = TOKEN_IDENTIFIER;

            while (is_identifier_symbol(tokenizer->cursor[0]))
                ++tokenizer->cursor;
            
            token.len = tokenizer->cursor - token.text;
        }
        break;
    }

    return token;
}

static size_t get_array_len(Token arr)
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

static Token get_array_element(Token arr, int index)
{
    Token token;
    token.text = arr.text;
    token.type = TOKEN_ERROR;
    token.len = arr.len;

    size_t arr_len = get_array_len(arr);

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

static int require_token(Tokenizer* tokenizer, TokenType type)
{
    return get_token(tokenizer).type == type;
}

static Token skip_till(Tokenizer* tokenizer, TokenType type)
{
    Token token = get_token(tokenizer);
    while (token.type != type)
    {
        if (token.type == TOKEN_END_OF_STREAM)
            break;

        token = get_token(tokenizer);
    }

    return token;
}

/* ---------------------------------------------------------------------------------------- */
static FILE* out_h;
static FILE* out_c;

static void parse_include(Tokenizer* tokenizer)
{
    Token block = skip_till(tokenizer, TOKEN_OPEN_BLOCK);
    for (size_t i = 0; i < block.len; ++i)
    {
        Token token = get_token(tokenizer);
        if (token.type == TOKEN_STRING)
            fprintf(out_h, "#include \"%.*s\"\n", token.len, token.text);
    }

    fprintf(out_h, "\n");
}

static void parse_enum(Tokenizer* tokenizer)
{
    skip_till(tokenizer, TOKEN_COLON);
    Token token_name = get_token(tokenizer);

    if (token_name.type != TOKEN_IDENTIFIER) return;

    fprintf(out_h, "typedef enum\n{\n");

    Token block = skip_till(tokenizer, TOKEN_OPEN_BLOCK);
    for (size_t i = 0; i < block.len; ++i)
    {
        Token token = get_token(tokenizer);
        if (token.type == TOKEN_ARRAY)
        {
            Token element = get_array_element(token, 0);
            fprintf(out_h, "\t%.*s", element.len, element.text);

            if (i < block.len - 1)
                fprintf(out_h, ",\n");
        }
    }
    
    fprintf(out_h, "\n} %.*s;\n\n", token_name.len, token_name.text);
}

static void parse_func(Tokenizer* tokenizer)
{
    Token token = get_token(tokenizer);
    if (token.type != TOKEN_IDENTIFIER) return;

    if (!require_token(tokenizer, TOKEN_OPEN_PAREN)) return;

    fprintf(out_h, "void %.*s(", token.len, token.text);
    fprintf(out_c, "void %.*s(", token.len, token.text);

    /* parse parameter */
    token = get_token(tokenizer);
    while (token.type != TOKEN_CLOSE_PAREN)
    {
        if (token.type == TOKEN_END_OF_STREAM)
        {
            printf("Reached end of stream\n");
            return;
        }

        fprintf(out_h, "%.*s", token.len, token.text);
        fprintf(out_c, "%.*s", token.len, token.text);

        token = get_token(tokenizer);
        if (token.type == TOKEN_IDENTIFIER)
        {
            fprintf(out_h, " ");
            fprintf(out_c, " ");
        }
    }

    fprintf(out_h, ");\n\n");
    fprintf(out_c, ")\n");
}

static void parse_func_body(Tokenizer* tokenizer)
{
    Token block = skip_till(tokenizer, TOKEN_OPEN_BLOCK);
    char* block_cursor = tokenizer->cursor;

    /* get function to call */
    skip_till(tokenizer, TOKEN_ARROW);
    char* func_def = tokenizer->cursor;

    for (size_t i = 0; i < block.len; ++i)
    {

        tokenizer->cursor = block_cursor;
        Token arr = get_token(tokenizer);
        block_cursor = tokenizer->cursor;

        if (arr.type != TOKEN_ARRAY) continue;

        tokenizer->cursor = func_def;

        Token token = get_token(tokenizer);
        if (!require_token(tokenizer, TOKEN_OPEN_PAREN)) return;

        fprintf(out_c, "\t%.*s(", token.len, token.text);

        int has_args = 0;

        /* parse arguments */
        token = get_token(tokenizer);
        while (token.type != TOKEN_CLOSE_PAREN)
        {
            if (token.type == TOKEN_END_OF_STREAM)
            {
                printf("Reached end of stream\n");
                return;
            }

            fprintf(out_c, "%.*s", token.len, token.text);
            has_args = 1;

            token = get_token(tokenizer);
        }

        size_t len = get_array_len(arr);
        for (int i = 1; i < len; ++i)
        {
            Token element = get_array_element(arr, i);

            if (!has_args)
            {
                fprintf(out_c, "%.*s", element.len, element.text);
                has_args = 1;
                continue;
            }
            
            fprintf(out_c, ", %.*s", element.len, element.text);
        }

        fprintf(out_c, ");\n");
    }

    return ;
    while(1)
    {
        Token token = get_token(tokenizer);
        if (!require_token(tokenizer, TOKEN_OPEN_PAREN)) return;

        fprintf(out_c, "%.*s(", token.len, token.text);

        /* parse arguments */
        token = get_token(tokenizer);
        while (token.type != TOKEN_CLOSE_PAREN)
        {
            if (token.type == TOKEN_END_OF_STREAM)
            {
                printf("Reached end of stream\n");
                return;
            }

            fprintf(out_c, "%.*s", token.len, token.text);

            token = get_token(tokenizer);
        }

        token = get_token(tokenizer);
        token_print(token);
        return;
        while(token.type != TOKEN_ARRAY)
        {
            if (token.type == TOKEN_END_OF_STREAM)
            {
                printf("Reached end of stream\n");
                return;
            }
            if (token.type == TOKEN_CLOSE_BLOCK)
            {
                return;
            }

            token = get_token(tokenizer);
        }

        size_t len = get_array_len(token);
        for (int i = 1; i < len; ++i)
        {
            Token element = get_array_element(token, i);
            fprintf(out_c, ", %.*s", element.len, element.text);
        }

        fprintf(out_c, ");\n");
    }
}

static void parse_generate(Tokenizer* tokenizer)
{
    char* reset = tokenizer->cursor;

    /* fist pass: enum */
    parse_enum(tokenizer);

    tokenizer->cursor = reset;

    /* second pass: function */
    parse_func(tokenizer);


    /* parse funtion body */
    fprintf(out_c, "{\n");
    parse_func_body(tokenizer);
    fprintf(out_c, "}\n\n");
}

int main(int argc, char** args)
{
    tb_file_error error;
    char* buffer = tb_file_read("EcsLoader.cx", "rb", &error);

    if (error != TB_FILE_OK)
    {
        printf("%s\n", tb_file_error_to_string(error));
        return 1;
    }

    out_h = fopen("EcsLoader.h", "w");
    out_c = fopen("EcsLoader.c", "w");

    if (!(out_c && out_h))
    {
        printf("Failed to open output file.\n");
        return 1;
    }

    fprintf(out_c, "#include \"%s\"\n\n", "EcsLoader.h");

    Tokenizer tokenizer;
    tokenizer.cursor = buffer;

    int parsing = 1;
    while (parsing)
    {
        Token token = get_token(&tokenizer);
        switch (token.type)
        {
        case TOKEN_END_OF_STREAM: parsing = 0; break;
        case TOKEN_ERROR: break;
        case TOKEN_IDENTIFIER:
            if (token_cmp(token, "include") == 0)
                parse_include(&tokenizer);

            if (token_cmp(token, "generate") == 0)
                parse_generate(&tokenizer);
            
            break;
        default:
            break;
        }
    }

    fclose(out_h);
    fclose(out_c);

    return 0;
}