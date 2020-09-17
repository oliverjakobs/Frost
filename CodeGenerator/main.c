#include <stdlib.h>

#define TB_FILE_IMPLEMENTATION
#include "tb_file.h"

#include <string.h>

#include "scanner.h"

/* ---------------------------------------------------------------------------------------- */
static FILE* out_h;
static FILE* out_c;

static void parse_include(Scanner* scanner)
{
    Token block = scanner_skip_till(scanner, TOKEN_OPEN_BLOCK);
    for (size_t i = 0; i < block.len; ++i)
    {
        Token token = scanner_get_next(scanner);
        if (token.type == TOKEN_STRING)
            fprintf(out_h, "#include \"%.*s\"\n", token.len, token.text);
    }

    fprintf(out_h, "\n");
}

static void parse_enum(Scanner* scanner)
{
    scanner_skip_till(scanner, TOKEN_COLON);
    Token token_name = scanner_get_next(scanner);

    if (token_name.type != TOKEN_IDENTIFIER) return;

    fprintf(out_h, "typedef enum\n{\n");

    Token block = scanner_skip_till(scanner, TOKEN_OPEN_BLOCK);
    for (size_t i = 0; i < block.len; ++i)
    {
        Token token = scanner_get_next(scanner);
        if (token.type == TOKEN_ARRAY)
        {
            Token element = token_array_at(token, 0);
            fprintf(out_h, "\t%.*s", element.len, element.text);

            if (i < block.len - 1)
                fprintf(out_h, ",\n");
        }
    }
    
    fprintf(out_h, "\n} %.*s;\n\n", token_name.len, token_name.text);
}

static void parse_func(Scanner* scanner)
{
    Token token = scanner_get_next(scanner);
    if (token.type != TOKEN_IDENTIFIER) return;

    if (!scanner_require(scanner, TOKEN_OPEN_PAREN)) return;

    fprintf(out_h, "void %.*s(", token.len, token.text);
    fprintf(out_c, "void %.*s(", token.len, token.text);

    /* parse parameter */
    token = scanner_get_next(scanner);
    while (token.type != TOKEN_CLOSE_PAREN)
    {
        if (token.type == TOKEN_END_OF_STREAM)
        {
            printf("Reached end of stream\n");
            return;
        }

        fprintf(out_h, "%.*s", token.len, token.text);
        fprintf(out_c, "%.*s", token.len, token.text);

        token = scanner_get_next(scanner);
        if (token.type == TOKEN_IDENTIFIER)
        {
            fprintf(out_h, " ");
            fprintf(out_c, " ");
        }
    }

    fprintf(out_h, ");\n\n");
    fprintf(out_c, ")\n");
}

static void parse_func_body(Scanner* scanner)
{
    /* get function to call */
    scanner_skip_till(scanner, TOKEN_ARROW);
    char* func_def = scanner->cursor;

    /* get block */
    Token block = scanner_skip_till(scanner, TOKEN_OPEN_BLOCK);
    char* block_cursor = scanner->cursor;

    for (size_t i = 0; i < block.len; ++i)
    {
        scanner->cursor = block_cursor;
        Token arr = scanner_get_next(scanner);
        block_cursor = scanner->cursor;

        if (arr.type != TOKEN_ARRAY) continue;

        scanner->cursor = func_def;

        Token token = scanner_get_next(scanner);
        if (!scanner_require(scanner, TOKEN_OPEN_PAREN)) return;

        fprintf(out_c, "\t%.*s(", token.len, token.text);

        int has_args = 0;

        /* parse arguments */
        token = scanner_get_next(scanner);
        while (token.type != TOKEN_CLOSE_PAREN)
        {
            if (token.type == TOKEN_END_OF_STREAM)
            {
                printf("Reached end of stream\n");
                return;
            }

            fprintf(out_c, "%.*s", token.len, token.text);
            has_args = 1;

            token = scanner_get_next(scanner);
        }

        size_t len = token_array_len(arr);
        for (int i = 1; i < len; ++i)
        {
            Token element = token_array_at(arr, i);

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
}

static void parse_generate(Scanner* scanner)
{
    char* reset = scanner->cursor;

    /* fist pass: enum */
    parse_enum(scanner);

    scanner->cursor = reset;

    /* second pass: function */
    parse_func(scanner);

    fprintf(out_c, "{\n");
    parse_func_body(scanner);
    fprintf(out_c, "}\n\n");
}

static Token define;

static void parse_define(Scanner* scanner)
{
    define = scanner_get_next(scanner);

    if (define.type != TOKEN_STRING)
    {
        printf("Expected string after define\n");
        return;
    }

    fprintf(out_h, "#ifndef %.*s\n", define.len, define.text);
    fprintf(out_h, "#define %.*s\n\n", define.len, define.text);
}

int main(int argc, char** args)
{
    tb_file_error error;
    char* buffer = tb_file_read("AnimationLoader.cx", "rb", &error);

    if (error != TB_FILE_OK)
    {
        printf("%s\n", tb_file_error_to_string(error));
        return 1;
    }

    out_h = fopen("AnimationLoader.h", "w");
    out_c = fopen("AnimationLoader.c", "w");

    if (!(out_c && out_h))
    {
        printf("Failed to open output file.\n");
        return 1;
    }

    fprintf(out_c, "#include \"%s\"\n\n", "AnimationLoader.h");

    Scanner scanner;
    scanner.cursor = buffer;

    int parsing = 1;
    while (parsing)
    {
        Token token = scanner_get_next(&scanner);
        switch (token.type)
        {
        case TOKEN_END_OF_STREAM: parsing = 0; break;
        case TOKEN_ERROR: break;
        case TOKEN_IDENTIFIER:
            if (token_cmp(token, "define") == 0)
                parse_define(&scanner);
            if (token_cmp(token, "include") == 0)
                parse_include(&scanner);
            if (token_cmp(token, "generate") == 0)
                parse_generate(&scanner);
            
            break;
        default:
            break;
        }
    }
    
    if (define.type == TOKEN_STRING)
        fprintf(out_h, "#endif /* !%.*s */\n", define.len, define.text);

    fclose(out_h);
    fclose(out_c);

    printf("Done\n");

    return 0;
}