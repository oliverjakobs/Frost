#include <stdio.h>
#include <string.h>

#include "generate.h"

/* TODO: source file */
/* TODO: error testing */

int main(int argc, char** args)
{
    Generator gen;

    /* 
     if (!generator_start(&gen, "AnimationLoader.cx", "AnimationLoader.h", "AnimationLoader.c"))
    */
    if (!generator_start(&gen, "EcsLoader.cx", "EcsLoader.h", "EcsLoader.c"))
    {
        printf("Failed to setup generator.\n");
        return 1;
    }

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

    generator_finish(&gen);

    return 0;
}