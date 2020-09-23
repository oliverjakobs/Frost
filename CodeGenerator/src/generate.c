#include "generate.h"

int generate_include(Generator* generator, size_t offset)
{
    generator_set_current(generator, offset);

    if (!generator_expect(generator, TOKEN_LEFT_BRACE, "Expected an '{'."))
        return 0;
    
    generator->current++;
    while (generator->current->type != TOKEN_RIGHT_BRACE)
    {
        /* TODO: Move to own func */
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

int generate_enum(Generator* generator, size_t offset)
{
    generator_set_current(generator, offset);

    if (!generator_expect(generator, TOKEN_IDENTIFIER, "Expected an indentifier."))
        return 0;

    GeneratorEnum* gen_enum = generator_get_enum(generator, generator->current);

    if (!gen_enum) return 0;

    
    fprintf(generator->out_header, "typedef enum\n{\n");

    generator->current = tb_array_get(&generator->tokens, gen_enum->offset);
    for (size_t i = 0; i < gen_enum->elements; ++i)
    {
        if (!generator_expect(generator, TOKEN_LEFT_BRACKET, "Expected '['."))
            return 0;
        
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

    fprintf(generator->out_header, "\n} %.*s;\n\n", gen_enum->token->len, gen_enum->token->start);
}

int generate_define_start(Generator* generator)
{
    generator_set_current(generator, 0);
    
    if (!generator_expect(generator, TOKEN_IDENTIFIER, "Expected an indentifier."))
        return 0;

    fprintf(generator->out_header, "#ifndef %.*s\n", generator->current->len, generator->current->start);
    fprintf(generator->out_header, "#define %.*s\n", generator->current->len, generator->current->start);

    fprintf(generator->out_header, "\n");
    return 1;
}

int generate_define_end(Generator* generator)
{
    generator_set_current(generator, 0);
    
    if (!generator_expect(generator, TOKEN_IDENTIFIER, "Expected an indentifier."))
        return 0;

    fprintf(generator->out_header, "#endif /* !%.*s */\n", generator->current->len, generator->current->start);
    return 1;
}

static int generate_strings(Generator* generator)
{
    if (!generator_expect(generator, TOKEN_COLON, "Expected ':'."))
        return 0;
    
    if (!generator_expect(generator, TOKEN_IDENTIFIER, "Expected an identifier."))
        return 0;

    GeneratorEnum* gen_enum = generator_get_enum(generator, generator->current);
    if (!gen_enum)
    {
        generator_error(generator, "Unknown enum.");
        return 0;
    }

    Token* token = gen_enum->token;

    /* write string func declarations */
    fprintf(generator->out_header, "const char* ");
    fprintf(generator->out_header, "%.*sToString", token->len, token->start);
    fprintf(generator->out_header, "(%.*s value);\n", token->len, token->start);

    fprintf(generator->out_header, "%.*s ", token->len, token->start);
    fprintf(generator->out_header, "%.*sFromString", token->len, token->start);
    fprintf(generator->out_header, "(const char* str);\n");

    fprintf(generator->out_header, "\n");

    /* write string func definitions */
    
    return 1;
}

int generate_func(Generator* generator, size_t offset)
{
    generator_set_current(generator, offset);

    /* get function type */
    generator->current++;
    if (token_cmp(generator->current, "strings") == 0)
        return generate_strings(generator);

    /* write func declaration */
    fprintf(generator->out_header, "void %.*s", generator->current->len, generator->current->start);

    if (!generator_expect(generator, TOKEN_LEFT_PAREN, "Expected '('."))
        return 0;

    fprintf(generator->out_header, "(");
    while (generator->current->type != TOKEN_RIGHT_PAREN)
    {
        /* TODO: generate parameter */
        generator->current++;
    }
    fprintf(generator->out_header, ");\n");


    fprintf(generator->out_header, "\n");
    return 1;
}