#include "generate.h"

int generate_include(Generator* generator, size_t offset)
{
    generator_set_current(generator, offset + 1);

    if (!generator_expect(generator, TOKEN_LEFT_BRACE, "Expected an '{'."))
        return 0;
    
    generator_advance(generator);
    while (generator->current->type != TOKEN_RIGHT_BRACE)
    {
        if (!generator_prevent(generator, TOKEN_LEFT_BRACE, "Unterminated include block."))
            return 0;

        if (generator_match(generator, TOKEN_STRING))
        {
            fprintf(generator->out_header, "#include %.*s\n", generator->current->len, generator->current->start);
        }
        else if (!generator_match(generator, TOKEN_COMMA))
        {
            generator_error(generator, "Unexpected token.");
            return 0;
        }

        generator_advance(generator);
    }

    fprintf(generator->out_header, "\n");

    return 1;
}

int generate_enum(Generator* generator, size_t offset)
{
    generator_set_current(generator, offset + 1);

    if (!generator_expect(generator, TOKEN_IDENTIFIER, "Expected an indentifier."))
        return 0;

    GeneratorEnum* gen_enum = generator_get_enum(generator, generator->current);

    if (!gen_enum) return 0;

    fprintf(generator->out_header, "typedef enum\n{\n");

    for (size_t i = 0; i < gen_enum->elements; ++i)
    {
        generator_enum_element(generator, gen_enum, i);

        fprintf(generator->out_header, "\t%.*s", generator->current->len, generator->current->start);

        if (i < gen_enum->elements - 1) fprintf(generator->out_header, ",\n");
    }

    Token* token = generator_enum_token(generator, gen_enum);
    fprintf(generator->out_header, "\n} %.*s;\n\n", token->len, token->start);

    return 1;
}

static int generate_strings(Generator* generator)
{
    generator_advance(generator);
    if (!generator_expect(generator, TOKEN_COLON, "Expected ':'."))
        return 0;

    generator_advance(generator);
    if (!generator_expect(generator, TOKEN_IDENTIFIER, "Expected an identifier."))
        return 0;

    GeneratorEnum* gen_enum = generator_get_enum(generator, generator->current);
    Token* token = generator_enum_token(generator, gen_enum);

    if (!(gen_enum && token))
    {
        generator_error(generator, "Unknown enum.");
        return 0;
    }

    /* write to_string declaration */
    fprintf(generator->out_header, "const char* ");
    fprintf(generator->out_header, "%.*sToString", token->len, token->start);
    fprintf(generator->out_header, "(%.*s value);\n", token->len, token->start);

    /* write to_string definition */
    fprintf(generator->out_source, "const char* ");
    fprintf(generator->out_source, "%.*sToString", token->len, token->start);
    fprintf(generator->out_source, "(%.*s value)\n{\n", token->len, token->start);

    for (size_t i = 0; i < gen_enum->elements; ++i)
    {
        generator_enum_element(generator, gen_enum, i);

        fprintf(generator->out_source, "\tif (value == %.*s) ", generator->current->len, generator->current->start);
        fprintf(generator->out_source, "return \"%.*s\";\n", generator->current->len, generator->current->start);
    }

    fprintf(generator->out_source, "\treturn NULL;\n");
    fprintf(generator->out_source, "}\n\n");

    /* write from_string declaration */
    fprintf(generator->out_header, "%.*s ", token->len, token->start);
    fprintf(generator->out_header, "%.*sFromString", token->len, token->start);
    fprintf(generator->out_header, "(const char* str);\n");

    /* write from_string definition */
    fprintf(generator->out_source, "%.*s ", token->len, token->start);
    fprintf(generator->out_source, "%.*sFromString", token->len, token->start);
    fprintf(generator->out_source, "(const char* str)\n{\n");

    for (size_t i = 0; i < gen_enum->elements; ++i)
    {
        generator_enum_element(generator, gen_enum, i);

        fprintf(generator->out_source, "\tif (strcmp(str, \"%.*s\") == 0) ", generator->current->len, generator->current->start);
        fprintf(generator->out_source, "return %.*s;\n", generator->current->len, generator->current->start);
    }

    fprintf(generator->out_source, "\treturn NULL;\n");
    fprintf(generator->out_source, "}\n\n");

    fprintf(generator->out_header, "\n");
    return 1;
}

int generate_func(Generator* generator, size_t offset)
{
    generator_set_current(generator, offset);

    /* get function type */
    generator_advance(generator);
    if (token_cmp(generator->current, "strings") == 0)
        return generate_strings(generator);

    /* write func declaration */
    fprintf(generator->out_header, "void %.*s", generator->current->len, generator->current->start);
    fprintf(generator->out_source, "void %.*s", generator->current->len, generator->current->start);

    generator_advance(generator);
    if (!generator_expect(generator, TOKEN_LEFT_PAREN, "Expected '('."))
        return 0;

    fprintf(generator->out_header, "(");
    fprintf(generator->out_source, "(");
    while (generator->current->type != TOKEN_RIGHT_PAREN)
    {
        /* TODO: generate parameter */
        generator_advance(generator);
    }
    fprintf(generator->out_header, ");\n\n");
    fprintf(generator->out_source, ")\n{\n");

    if (!generator_expect(generator, TOKEN_RIGHT_PAREN, "Expected ')'."))
        return 0;

    generator_advance(generator);
    if (!generator_expect(generator, TOKEN_COLON, "Expected ':'."))
        return 0;

    generator_advance(generator);

    GeneratorEnum* gen_enum = generator_get_enum(generator, generator->current);
    Token* token = generator_enum_token(generator, gen_enum);

    if (!(gen_enum && token))
    {
        generator_error(generator, "Unknown enum.");
        return 0;
    }

    /* Get function to call */
    generator_advance(generator);
    if (!generator_expect(generator, TOKEN_ARROW, "Expected '->'."))
        return 0;

    Token* func = ++generator->current;

    /* write func body */
    for (size_t i = 0; i < gen_enum->elements; ++i)
    {
        generator_enum_element(generator, gen_enum, i);

        fprintf(generator->out_source, "\t%.*s(", func->len, func->start);
        generator_advance(generator);
        for (size_t arg = 0; arg < gen_enum->element_size;)
        {
            /* TODO: generate parameter */
            generator_advance(generator);

            if (generator_match(generator, TOKEN_RIGHT_BRACKET))
                break;

            if (generator_match(generator, TOKEN_COMMA))
                fprintf(generator->out_source, ", ");
            else
            {
                fprintf(generator->out_source, "%.*s", generator->current->len, generator->current->start);
                arg++;
            }
        }
        fprintf(generator->out_source, ");\n");
    }

    fprintf(generator->out_source, "}\n\n");

    return 1;
}