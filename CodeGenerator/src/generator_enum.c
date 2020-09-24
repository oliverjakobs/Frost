#include "generator_enum.h"

#include <string.h>

int generator_enum_init(Generator* generator, GeneratorEnum* gen_enum, size_t offset)
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

        /* TODO: check commas after args */
        size_t size = 0;
        
        generator_advance(generator);
        while (generator->current->type != TOKEN_RIGHT_BRACKET)
        {
            if (!generator_prevent(generator, TOKEN_LEFT_BRACKET, "Unterminated enum element."))
                return 0;

            generator_advance(generator);
            size++;
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

Token* generator_enum_token(Generator* generator, GeneratorEnum* gen_enum)
{
    if (!gen_enum) return NULL;

    return tb_array_get(&generator->tokens, gen_enum->offset);
}

GeneratorEnum* generator_enum_get(Generator* generator, Token* token)
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

int generator_enum_element(Generator* generator, GeneratorEnum* gen_enum, size_t index)
{
    if (index >= gen_enum->elements)
        return 0;

    size_t offset = gen_enum->offset + 3 + (index * (gen_enum->element_size + 3));
    generator_set_current(generator, offset);

    return 1;
}
