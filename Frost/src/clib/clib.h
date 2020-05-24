#ifndef CLIB_H
#define CLIB_H

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

#define BETWEEN(start, end, value) (((value) >= (start)) && ((value) <= (end)))

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

#endif /* !CLIB_H */
