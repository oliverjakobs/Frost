#ifndef GENERATOR_H
#define GENERATOR_H

#include "scanner.h"

#include "tb_array.h"

#include <stdio.h>

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

void generator_advance(Generator* generator);
void generator_set_current(Generator* generator, size_t offset);
size_t generator_offset(Generator* generator, Token* token);
Token* generator_at(Generator* generator, size_t index);


#endif /* !GENERATOR_H */