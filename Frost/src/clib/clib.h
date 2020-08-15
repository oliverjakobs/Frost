#ifndef CLIB_H
#define CLIB_H

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/*
 * Define CLIB_NOASSERT to compile out all assertions used internally.
 */
/* #define CLIB_NOASSERT */
#ifndef CLIB_NOASSERT
#include <assert.h>
#define CLIB_ASSERT(expr) assert(expr)
#else
#define CLIB_ASSERT(expr)
#endif

void clib_swap(void** a, void** b);
void clib_swapi(int* a, int* b);
void clib_swapf(float* a, float* b);
void clib_swapstr(char** a, char** b);

#endif /* !CLIB_H */
