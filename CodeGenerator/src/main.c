#include <stdio.h>
#include <string.h>

#include "generate.h"

/* TODO: error testing */

int main(int argc, char** args)
{
    char* script = "EcsLoader.cx";
    /*
    char* script = "AnimationLoader.cx";
    */

    Generator gen;
    if (!generator_start(&gen, script))
    {
        printf("Failed to setup generator.\n");
        return 1;
    }

    for (size_t i = 0; i < gen.tokens.used; ++i)
    {
        Token* token = generator_at(&gen, i);
        switch (token->type)
        {
        case TOKEN_INCLUDE:     generate_include(&gen, i); break;
        case TOKEN_GENERATE:    generate_func(&gen, i); break;
        case TOKEN_ENUM:        generate_enum(&gen, i); break;
        }
    }

    generator_finish(&gen);

    return 0;
}