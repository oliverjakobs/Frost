#ifndef GENERATE_H
#define GENERATE_H

#include "generator.h"

int generate_include(Generator* generator, size_t offset);
int generate_enum(Generator* generator, size_t offset);

int generate_func(Generator* generator, size_t offset);

#endif /* !GENERATE_H */