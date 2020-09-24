#include "generator.h"

#define TB_FILE_IMPLEMENTATION
#include "tb_file.h"

#include <errno.h>

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

static int generator_load_enum(Generator* generator, GeneratorEnum* gen_enum, size_t offset)
{
    generator_set_current(generator, offset);

    generator_advance(generator);
    if (!generator_expect(generator, TOKEN_IDENTIFIER, "Expected an indentifier."))
        return 0;

    generator_advance(generator);
    if (!generator_expect(generator, TOKEN_LEFT_BRACE, "Missing '{' after enum."))
        return 0;

    gen_enum->offset = offset + 1;
    gen_enum->elements = 0;
    gen_enum->element_size = 0;

    while (generator->current->type != TOKEN_RIGHT_BRACE)
    {
        generator_advance(generator);
        if (!generator_expect(generator, TOKEN_LEFT_BRACKET, "Enum elements must start with '['."))
            return 0;

        /* TODO: check comments after args */
        size_t size = 0;
        
        generator_advance(generator);
        while (generator->current->type != TOKEN_RIGHT_BRACKET)
        {
            if (!generator_prevent(generator, TOKEN_LEFT_BRACKET, "Unterminated enum element."))
                return 0;

            size++;

            generator_advance(generator);
        }

        if (size == 0)
        {
            generator_error(generator, "Enum element missing argumnets.");
            return 0;
        }
        else if (gen_enum->element_size > 0 && gen_enum->element_size != size)
        {
            generator_error(generator, "Enum element size missmatch.");
            return 0;
        }
        else
        {
            gen_enum->element_size = size;
        }

        if (!(generator_match_next(generator, TOKEN_COMMA) 
            || generator_match_next(generator, TOKEN_RIGHT_BRACE)))
        {
            generator_error(generator, "Missing ',' after enum element.");
            return 0;
        }

        generator_advance(generator);
        gen_enum->elements++;
    }

    return 1;
}

int generator_start(Generator* generator, const char* script, const char* header, const char* source)
{
    tb_file_error error;
    generator->file_buffer = tb_file_read(script, "rb", &error);

    if (error != TB_FILE_OK)
    {
        printf("%s: %s\n", tb_file_error_to_string(error), strerror(errno));
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

    /* open output */
    generator->out_header = fopen(header, "w");
    generator->out_source = fopen(source, "w");

    if (!(generator->out_header && generator->out_source))
    {
        printf("Failed to open output files: %s\n",strerror(errno));
        return 0;
    }

    /* load define */
    if (generator->current->type != TOKEN_DEFINE)
    {
        generator_error(generator, "Missing define at start of file.");
        return 0;
    }
    
    generator_advance(generator);
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

    /* start include guard */
    generator_set_current(generator, 0);
    if (!generator_expect(generator, TOKEN_DEFINE, "Expected define at beginning of script."))
        return 0;
    
    generator_advance(generator);
    if (!generator_expect(generator, TOKEN_IDENTIFIER, "Expected an indentifier."))
        return 0;

    fprintf(generator->out_header, "#ifndef %.*s\n", generator->current->len, generator->current->start);
    fprintf(generator->out_header, "#define %.*s\n", generator->current->len, generator->current->start);

    fprintf(generator->out_header, "\n");

    /* generate include for source files */
    fprintf(generator->out_source, "#include \"%s\"\n", header);

    /* TODO: dynamic includes */
    fprintf(generator->out_source, "#include <%s>\n", "string.h");
    fprintf(generator->out_source, "\n");

    return 1;
}

void generator_finish(Generator* generator)
{
    /* close include guard */
    generator_set_current(generator, 0);
    fprintf(generator->out_header, "#endif /* !%.*s */\n", generator->current->len, generator->current->start);

    /* clean up */
    if (generator->out_header) fclose(generator->out_header);
    if (generator->out_source) fclose(generator->out_source);

    tb_array_free(&generator->tokens);
    tb_array_free(&generator->enums);
    free(generator->file_buffer);
}

int generator_match(Generator* generator, TokenType type)
{
    return generator->current->type == type;
}

int generator_match_next(Generator* generator, TokenType type)
{
    Token* next = generator->current + 1;
    return next->type == type;
}

void generator_error(Generator* generator, const char* msg)
{
    printf("%s (%d)\n", msg, generator->current->line);
}

int generator_expect(Generator* generator, TokenType type, const char* msg)
{
    if (!generator_match(generator, type))
    {
        generator_error(generator, msg);
        return 0;
    }
    
    return 1;
}

int generator_prevent(Generator* generator, TokenType type, const char* msg)
{
    if (generator_match(generator, TOKEN_EOF) || generator_match(generator, TOKEN_ERROR)
    || generator_match(generator, type))
    {
        generator_error(generator, msg);
        return 0;
    }
    
    return 1;
}

Token* generator_enum_token(Generator* generator, GeneratorEnum* gen_enum)
{
    if (!gen_enum) return NULL;

    return tb_array_get(&generator->tokens, gen_enum->offset);
}

int generator_enum_element(Generator* generator, GeneratorEnum* gen_enum, size_t index)
{
    if (index >= gen_enum->elements)
        return 0;

    size_t offset = gen_enum->offset + 3 + (index * (gen_enum->element_size + 3));
    generator_set_current(generator, offset);

    return 1;
}


GeneratorEnum* generator_get_enum(Generator* generator, Token* token)
{
    for (size_t i = 0; i < generator->enums.used; ++i)
    {
        GeneratorEnum* e = tb_array_get(&generator->enums, i);
        Token* t = generator_enum_token(generator, e);
        if (t->len == token->len && memcmp(t->start, token->start, t->len) == 0)
            return e;
    }

    return NULL;
}

void generator_advance(Generator* generator)
{
    generator->current++;
}

void generator_set_current(Generator* generator, size_t offset)
{
    if (offset >= generator->tokens.used)
    {
        printf("Offset out of range.\n");
        return;
    }

    generator->current = tb_array_get(&generator->tokens, offset);
}

size_t generator_get_offset(Generator* generator, Token* token)
{
    return token - (Token*)tb_array_first(&generator->tokens);
}