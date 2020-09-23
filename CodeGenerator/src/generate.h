#ifndef GENERATE_H
#define GENERATE_H

#include "generator.h"

int generate_include(Generator* generator, size_t offset);
int generate_enum(Generator* generator, size_t offset);

int generate_source_include(Generator* generator);
int generate_define_start(Generator* generator);
int generate_define_end(Generator* generator);

int generate_func(Generator* generator, size_t offset);

#endif /* !GENERATE_H */