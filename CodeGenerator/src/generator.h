#ifndef GENERATOR_H
#define GENERATOR_H

#include "scanner.h"

#include <stdio.h>

typedef struct
{
    const char* name;
    size_t name_len;

    size_t offset;

    size_t elements;
} GeneratorEnum;

typedef struct
{
    tb_array tokens;
    Token* current;

    tb_array enums;

    char* file_buffer;
    FILE* out_header;
    FILE* out_source;
} Generator;

int generator_create_tokens(Generator* generator, const char* path);
void generator_free(Generator* generator);

void generator_error(Generator* generator, const char* msg);
int generator_expect(Generator* generator, TokenType type, const char* msg);

int generator_prime(Generator* generator, const char* header, const char* source);

GeneratorEnum* generator_get_enum(Generator* generator, Token* token);


#endif /* !GENERATOR_H */