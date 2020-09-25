#include "generator.h"

#define TB_FILE_IMPLEMENTATION
#include "tb_file.h"

#define TB_ARRAY_IMPLEMENTATION
#include "tb_array.h"

#include "generator_enum.h"

#include <errno.h>

static int generator_load_tokens(Generator* generator, Scanner* scanner)
{
    if (tb_array_alloc(&generator->tokens, 32, sizeof(Token), 1.2f) != TB_ARRAY_OK)
    {
        printf("Failed to allocate token array.\n");
        return 0;
    }

    while (1)
    {
        Token token = scanner_get_next(scanner);

        if (!tb_array_push(&generator->tokens, &token))
        {
            printf("Failed to add token (%.*s) to array.\n", token.len, token.start);
            return 0;
        }

        if (token.type == TOKEN_EOF) break;
    }

    if (tb_array_shrink_to_fit(&generator->tokens) != TB_ARRAY_OK)
    {
        printf("Failed to shrink token array.\n");
        tb_array_free(&generator->tokens);
        return 0;
    }

    return 1;
}

static int generator_load_enums(Generator* generator)
{
    if (tb_array_alloc(&generator->enums, 8, sizeof(GeneratorEnum), 1.2f) != TB_ARRAY_OK)
    {
        printf("Failed to allocate array for enums.\n");
        return 0;
    }

    for (size_t i = 0; i < generator->tokens.used; ++i)
    {
        generator_set_current(generator, i);
        if (generator_match(generator, TOKEN_ENUM))
        {
            GeneratorEnum gen_enum;
            if (!generator_enum_init(generator, &gen_enum, i))
            {
                printf("Failed to init enum.\n");
                return 0;
            }

            if (!tb_array_push(&generator->enums, &gen_enum))
            {
                Token* token = generator_enum_token(generator, &gen_enum);
                printf("Failed to add enum (%.*s) to array.\n", token->len, token->start);
                return 0;
            }
        }
    }

    if (tb_array_shrink_to_fit(&generator->enums) != TB_ARRAY_OK)
    {
        printf("Failed to shrink array for enums.\n");
        return 0;
    }

    return 1;
}

static char* generator_get_output(const char* script, const char* ext)
{
    const char* start = strrchr(script, '\\') + 1;

    if (!start) start = script;

    size_t ext_len = strlen(ext);
    size_t len = strcspn(start, ".") + 1;

    char* path = malloc(len + ext_len + 1);
    if (!path) return NULL;

    strncpy(path, start, len);
    strncpy(path + len, ext, ext_len);

    path[len + ext_len] = '\0';

    return path;
}

static void generator_free(Generator* generator)
{
    if (generator->out_header) fclose(generator->out_header);
    if (generator->out_source) fclose(generator->out_source);

    tb_array_free(&generator->tokens);
    tb_array_free(&generator->enums);

    if (generator->file_buffer) free(generator->file_buffer);
}

int generator_start(Generator* generator, const char* script)
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
        printf("Failed to load tokens.\n");
        generator_free(generator);
        return 0;
    }

    if (!generator_load_enums(generator))
    {
        printf("Failed to load enums.\n");
        generator_free(generator);
        return 0;
    }

    /* check for define at beginning of the file */
    generator_set_current(generator, 0);
    if (!generator_expect(generator, TOKEN_DEFINE, "Expected define at beginning of script."))
    {
        generator_free(generator);
        return 0;
    }
    
    generator_advance(generator);
    if (!generator_expect(generator, TOKEN_IDENTIFIER, "Expected an indentifier."))
    {
        generator_free(generator);
        return 0;
    }

    /* open output */
    char* header = generator_get_output(script, "h");
    char* source = generator_get_output(script, "c");

    generator->out_header = fopen(header, "w");
    generator->out_source = fopen(source, "w");

    if (!(generator->out_header && generator->out_source))
    {
        printf("Failed to open output files: %s\n", strerror(errno));
        generator_free(generator);
        free(header);
        free(source);
        return 0;
    }

    /* start include guard */
    fprintf(generator->out_header, "#ifndef %.*s\n", generator->current->len, generator->current->start);
    fprintf(generator->out_header, "#define %.*s\n", generator->current->len, generator->current->start);

    fprintf(generator->out_header, "\n");

    /* generate include for source files */
    fprintf(generator->out_source, "#include \"%s\"\n", header);

    /* TODO: dynamic includes */
    fprintf(generator->out_source, "#include <%s>\n", "string.h");
    fprintf(generator->out_source, "\n");

    free(header);
    free(source);

    return 1;
}

void generator_finish(Generator* generator)
{
    /* close include guard */
    generator_set_current(generator, 0);
    fprintf(generator->out_header, "#endif /* !%.*s */\n", generator->current->len, generator->current->start);

    /* clean up */
    generator_free(generator);
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

void generator_advance(Generator* generator)
{
    if (generator_offset(generator, generator->current) >= generator->tokens.used)
    {
        printf("Reached end of file.\n");
        return;
    }

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

size_t generator_offset(Generator* generator, Token* token)
{
    return token - (Token*)tb_array_first(&generator->tokens);
}

Token* generator_at(Generator* generator, size_t index)
{
    return tb_array_get(&generator->tokens, index);
}