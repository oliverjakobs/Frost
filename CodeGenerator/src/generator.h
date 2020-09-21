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

int generator_prime(Generator* generator, const char* header, const char* source);

/* generate functions */
int generate_include(Generator* generator, size_t offset);
int generate_func(Generator* generator, size_t offset);
int generate_enum(Generator* generator, size_t offset);
int generate_define_start(Generator* generator);
int generate_define_end(Generator* generator);

#endif /* !GENERATOR_H */