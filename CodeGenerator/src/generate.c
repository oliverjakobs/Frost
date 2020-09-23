#include "generate.h"

int generate_include(Generator* generator, size_t offset)
{
    generator_set_current(generator, offset);

    if (!generator_expect(generator, TOKEN_LEFT_BRACE, "Expected an '{'."))
        return 0;
    
    generator->current++;
    while (generator->current->type != TOKEN_RIGHT_BRACE)
    {
        if (!generator_prevent(generator, TOKEN_LEFT_BRACE))
        {
            generator_error(generator, "Unterminated include block.");
            return 0;
        }

        if (generator_match(generator, TOKEN_STRING))
        {
            fprintf(generator->out_header, "#include %.*s\n", generator->current->len, generator->current->start);
        }
        else if (!generator_match(generator, TOKEN_COMMA))
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

    generator_set_current(generator, gen_enum->offset + 3);
    for (size_t i = 0; i < gen_enum->elements; ++i)
    {
        fprintf(generator->out_header, "\t%.*s", generator->current->len, generator->current->start);

        if (i < gen_enum->elements - 1) fprintf(generator->out_header, ",\n");

        generator_enum_next(generator, gen_enum);
    }

    Token* token = generator_enum_token(generator, gen_enum);
    fprintf(generator->out_header, "\n} %.*s;\n\n", token->len, token->start);
}

int generate_source_include(Generator* generator)
{
    /* TODO: out of here */
    char* file = "AnimationLoader.h";

    fprintf(generator->out_source, "#include \"%s\"\n", file);

    fprintf(generator->out_source, "#include <%s>\n", "string.h");
    fprintf(generator->out_source, "\n");
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

    Token* token = generator_enum_token(generator, gen_enum);

    /* write to_string declaration */
    fprintf(generator->out_header, "const char* ");
    fprintf(generator->out_header, "%.*sToString", token->len, token->start);
    fprintf(generator->out_header, "(%.*s value);\n", token->len, token->start);

    /* write to_string definition */
    fprintf(generator->out_source, "const char* ");
    fprintf(generator->out_source, "%.*sToString", token->len, token->start);
    fprintf(generator->out_source, "(%.*s value)\n{\n", token->len, token->start);

    generator_set_current(generator, gen_enum->offset + 3);
    for (size_t i = 0; i < gen_enum->elements; ++i)
    {
        fprintf(generator->out_source, "\tif (value == %.*s) ", generator->current->len, generator->current->start);
        fprintf(generator->out_source, "return \"%.*s\";\n", generator->current->len, generator->current->start);

        generator_enum_next(generator, gen_enum);
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

    generator_set_current(generator, gen_enum->offset + 3);
    for (size_t i = 0; i < gen_enum->elements; ++i)
    {
        fprintf(generator->out_source, "\tif (strcmp(str, \"%.*s\") == 0) ", generator->current->len, generator->current->start);
        fprintf(generator->out_source, "return %.*s;\n", generator->current->len, generator->current->start);

        generator_enum_next(generator, gen_enum);
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