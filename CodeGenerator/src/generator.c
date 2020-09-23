#include "generator.h"

#define TB_FILE_IMPLEMENTATION
#include "tb_file.h"

static int generator_load_tokens(Generator* generator, Scanner* scanner)
{
    if (tb_array_alloc(&generator->tokens, 32, sizeof(Token), 1.2f) != TB_ARRAY_OK)
        return 0;

    while (1)
    {
        Token token = scanner_get_next(scanner);

        if (!tb_array_push(&generator->tokens, &token))
        {
            printf("Failed to add token (%.*s) to array\n", token.len, token.start);
            break;
        }

        if (token.type == TOKEN_EOF) break;
    }

    if (tb_array_shrink_to_fit(&generator->tokens) != TB_ARRAY_OK)
    {
        tb_array_free(&generator->tokens);
        return 0;
    }

    return 1;
}

int generator_create_tokens(Generator* generator, const char* path)
{
    tb_file_error error;
    generator->file_buffer = tb_file_read(path, "rb", &error);

    if (error != TB_FILE_OK)
    {
        printf("%s\n", tb_file_error_to_string(error));
        return 0;
    }

    Scanner scanner;
    scanner_init(&scanner, generator->file_buffer);

    if (!generator_load_tokens(generator, &scanner))
    {
        free(generator->file_buffer);
        return 0;
    } 

    generator->current = tb_array_first(&generator->tokens);

    return 1;
}

void generator_free(Generator* generator)
{
    if (generator->out_header) fclose(generator->out_header);
    if (generator->out_source) fclose(generator->out_source);

    tb_array_free(&generator->tokens);
    tb_array_free(&generator->enums);
    free(generator->file_buffer);
}

void generator_error(Generator* generator, const char* msg)
{
    printf("%s (%d)\n", msg, generator->current->line);
}

int generator_expect(Generator* generator, TokenType type, const char* msg)
{
    generator->current++;
    if (generator->current->type != type)
    {
        generator_error(generator, msg);
        return 0;
    }

    return 1;
}

static int count_enum_elements(Generator* generator)
{
    if (!generator_expect(generator, TOKEN_LEFT_BRACKET, "Enum elements must start with '['."))
        return 0;

    while (generator->current->type != TOKEN_RIGHT_BRACKET)
    {
        generator->current++;

        if (generator->current->type == TOKEN_EOF 
            || generator->current->type == TOKEN_ERROR
            || generator->current->type == TOKEN_LEFT_BRACKET)
        {
            generator_error(generator, "Unterminated enum element.");
            return 0;
        }
    }

    return 1;
}

static int generator_load_enum(Generator* generator, GeneratorEnum* gen_enum, size_t offset)
{
    generator->current = tb_array_get(&generator->tokens, offset);

    if (!generator_expect(generator, TOKEN_IDENTIFIER, "Expected an indentifier."))
        return 0;

    gen_enum->name = generator->current->start;
    gen_enum->name_len = generator->current->len;

    if (!generator_expect(generator, TOKEN_LEFT_BRACE, "Missing '{' after enum."))
        return 0;

    gen_enum->offset = generator->current - (Token*)tb_array_first(&generator->tokens);
    gen_enum->elements = 0;

    while (generator->current->type != TOKEN_RIGHT_BRACE)
    {
        if (!count_enum_elements(generator))
            return 0;

        gen_enum->elements++;
        generator->current++;
    }

    return 1;
}

int generator_prime(Generator* generator, const char* header, const char* source)
{
    generator->out_header = fopen(header, "w");
    generator->out_source = fopen(source, "w");

    if (!(generator->out_header && generator->out_source))
    {
        printf("Failed to open output files.\n");
        return 0;
    }

    /* load define */
    if (generator->current->type != TOKEN_DEFINE)
    {
        generator_error(generator, "Missing define at start of file.");
        return 0;
    }

    if (!generator_expect(generator, TOKEN_IDENTIFIER, "Expected an indentifier."))
        return 0;

    /* load enums */
    if (tb_array_alloc(&generator->enums, 8, sizeof(GeneratorEnum), 1.2f) != TB_ARRAY_OK)
    {
        printf("Failed to allocate array for enums.\n");
        return 0;
    }

    for (size_t i = 0; i < generator->tokens.used; ++i)
    {
        Token* token = tb_array_get(&generator->tokens, i);

        if (token->type == TOKEN_ENUM)
        {
            GeneratorEnum gen_enum;
            if (generator_load_enum(generator, &gen_enum, i))
                tb_array_push(&generator->enums, &gen_enum);
        }
    }

    if (tb_array_shrink_to_fit(&generator->enums) != TB_ARRAY_OK)
    {
        tb_array_free(&generator->enums);
        printf("Failed to shrink array for enums.\n");
        return 0;
    }

    return 1;
}

GeneratorEnum* generator_get_enum(Generator* generator, Token* token)
{
    if (!token) token = generator->current;

    for (size_t i = 0; i < generator->enums.used; ++i)
    {
        GeneratorEnum* e = tb_array_get(&generator->enums, i);
        if (e->name_len == token->len && memcmp(e->name, token->start, e->name_len) == 0)
            return e;
    }

    return NULL;
}