#include <stdio.h>
#include <string.h>

#include "generate.h"

/* TODO: source file */
/* TODO: error testing */

int main(int argc, char** args)
{
    Generator gen;
    if (!generator_create_tokens(&gen, "AnimationLoader.cx"))
    {
        printf("Failed to create tokens\n");
        return 1;
    }

    if (!generator_prime(&gen, "AnimationLoader.h", "AnimationLoader.c"))
    {
        printf("Failed to prime generator.\n");
        return 1;
    }

    generate_define_start(&gen);

    for (size_t i = 0; i < gen.tokens.used; ++i)
    {
        Token* token = tb_array_get(&gen.tokens, i);
        switch (token->type)
        {
        case TOKEN_INCLUDE:     generate_include(&gen, i); break;
        case TOKEN_GENERATE:    generate_func(&gen, i); break;
        case TOKEN_ENUM:        generate_enum(&gen, i); break;
        }
    }

    generate_define_end(&gen);

    generator_free(&gen);

    return 0;
}