#ifndef GENERATOR_H
#define GENERATOR_H

#include "scanner.h"

#include <stdio.h>

typedef struct
{
    size_t offset;
    size_t elements;
    size_t element_size;
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

int generator_start(Generator* generator, const char* script);
void generator_finish(Generator* generator);

int generator_match(Generator* generator, TokenType type);
int generator_match_next(Generator* generator, TokenType type);

void generator_error(Generator* generator, const char* msg);

int generator_expect(Generator* generator, TokenType type, const char* msg);
int generator_prevent(Generator* generator, TokenType type, const char* msg);

Token* generator_enum_token(Generator* generator, GeneratorEnum* gen_enum);
int generator_enum_element(Generator* generator, GeneratorEnum* gen_enum, size_t index);

GeneratorEnum* generator_get_enum(Generator* generator, Token* token);

void generator_advance(Generator* generator);
void generator_set_current(Generator* generator, size_t offset);
size_t generator_get_offset(Generator* generator, Token* token);



#endif /* !GENERATOR_H */