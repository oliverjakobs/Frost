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

static void generator_error(Generator* generator, const char* msg)
{
    printf("%s (%d)\n", msg, generator->current->line);
}

static int count_enum_elements(Generator* generator)
{
    generator->current++;

    if (generator->current->type != TOKEN_LEFT_BRACKET)
    {
        generator_error(generator, "Enum elements must start with '['.");
        return 0;
    }

    generator->current++;
    while (generator->current->type != TOKEN_RIGHT_BRACKET)
    {
        if (generator->current->type == TOKEN_EOF 
            || generator->current->type == TOKEN_ERROR
            || generator->current->type == TOKEN_LEFT_BRACKET)
        {
            generator_error(generator, "Unterminated enum element.");
            return 0;
        }

        generator->current++;
    }

    return 1;
}

static int generator_load_enum(Generator* generator, GeneratorEnum* gen_enum, size_t offset)
{
    generator->current = tb_array_get(&generator->tokens, offset);

    generator->current++;
    if (generator->current->type != TOKEN_IDENTIFIER)
    {
        generator_error(generator, "Expected an indentifier.");
        return 0;
    }

    gen_enum->name = generator->current->start;
    gen_enum->name_len = generator->current->len;

    generator->current++;
    if (generator->current->type != TOKEN_LEFT_BRACE)
    {
        generator_error(generator, "Missing '{' after enum.");
        return 0;
    }

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

static GeneratorEnum* generator_get_enum(Generator* generator, const char* name, size_t len)
{
    for (size_t i = 0; i < generator->enums.used; ++i)
    {
        GeneratorEnum* e = tb_array_get(&generator->enums, i);
        if (e->name_len == len && memcmp(e->name, name, e->name_len) == 0)
            return e;
    }

    return NULL;
}

int generate_include(Generator* generator, size_t offset)
{
    generator->current = tb_array_get(&generator->tokens, offset);
    generator->current++;

    if (generator->current->type != TOKEN_LEFT_BRACE)
    {
        generator_error(generator, "Expected an '{'.");
        return 0;
    }
    
    generator->current++;
    while (generator->current->type != TOKEN_RIGHT_BRACE)
    {
        if (generator->current->type == TOKEN_EOF 
            || generator->current->type == TOKEN_ERROR
            || generator->current->type == TOKEN_LEFT_BRACE)
        {
            generator_error(generator, "Unterminated include block.");
            return 0;
        }

        if (generator->current->type == TOKEN_STRING)
        {
            fprintf(generator->out_header, "#include %.*s\n", generator->current->len, generator->current->start);
        }
        else if (generator->current->type != TOKEN_COMMA)
        {
            generator_error(generator, "Unexpected token.");
            return 0;
        }

        generator->current++;
    }

    fprintf(generator->out_header, "\n");

    return 1;
}

int generate_func(Generator* generator, size_t offset)
{

}

int generate_enum(Generator* generator, size_t offset)
{
    generator->current = tb_array_get(&generator->tokens, offset);

    generator->current++;
    if (generator->current->type != TOKEN_IDENTIFIER)
    {
        generator_error(generator, "Expected an indentifier.");
        return 0;
    }

    GeneratorEnum* gen_enum = generator_get_enum(generator, generator->current->start, generator->current->len);

    if (!gen_enum)
        return 0;

    
    fprintf(generator->out_header, "typedef enum\n{\n");

    generator->current = tb_array_get(&generator->tokens, gen_enum->offset);
    for (size_t i = 0; i < gen_enum->elements; ++i)
    {
        generator->current++;
        if (generator->current->type != TOKEN_LEFT_BRACKET)
        {
            generator_error(generator, "Expected '['.");
            return 0;
        }
        
        generator->current++;

        fprintf(generator->out_header, "\t%.*s", generator->current->len, generator->current->start);

        while (generator->current->type != TOKEN_RIGHT_BRACKET)
        {
            if (generator->current->type == TOKEN_EOF 
                || generator->current->type == TOKEN_ERROR
                || generator->current->type == TOKEN_LEFT_BRACKET)
            {
                generator_error(generator, "Unterminated enum element.");
                return 0;
            }

            generator->current++;
        }

        if (i < gen_enum->elements - 1)
                fprintf(generator->out_header, ",\n");

        generator->current++;
    }

    fprintf(generator->out_header, "\n} %.*s;\n\n", gen_enum->name_len, gen_enum->name);
}

int generate_define_start(Generator* generator)
{

}

int generate_define_end(Generator* generator)
{

}