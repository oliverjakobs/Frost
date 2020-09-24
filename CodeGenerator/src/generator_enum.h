#ifndef GENERATOR_ENUM_H
#define GENERATOR_ENUM_H

#include "generator.h"

typedef struct
{
    size_t offset;
    size_t elements;
    size_t element_size;
} GeneratorEnum;

int generator_enum_init(Generator* generator, GeneratorEnum* gen_enum, size_t offset);

Token* generator_enum_token(Generator* generator, GeneratorEnum* gen_enum);
GeneratorEnum* generator_enum_get(Generator* generator, Token* token);

int generator_enum_element(Generator* generator, GeneratorEnum* gen_enum, size_t index);

#endif /* !GENERATOR_ENUM_H */