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

#include <stdint.h>

void clib_swap(void** a, void** b);
void clib_swap_i(int* a, int* b);
void clib_swap_i32(int32_t* a, int32_t* b);
void clib_swap_i64(int64_t* a, int64_t* b);
void clib_swap_f(float* a, float* b);
void clib_swap_str(char** a, char** b);

inline int clib_max_i(int a, int b) { return a > b ? a : b; }
inline int32_t clib_max_i32(int32_t a, int32_t b) { return a > b ? a : b; }
inline int64_t clib_max_i64(int64_t a, int64_t b) { return a > b ? a : b; }
inline float clib_max_f(float a, float b) { return a > b ? a : b; }

inline int clib_min_i(int a, int b) { return a < b ? a : b; }
inline int32_t clib_min_i32(int32_t a, int32_t b) { return a < b ? a : b; }
inline int64_t clib_min_i64(int64_t a, int64_t b) { return a < b ? a : b; }
inline float clib_min_f(float a, float b) { return a < b ? a : b; }

inline uint8_t clib_between_i(int start, int end, int value) { return (value >= start) && (value <= end); }
inline uint8_t clib_between_i32(int start, int end, int value) { return (value >= start) && (value <= end); }
inline uint8_t clib_between_i64(int start, int end, int value) { return (value >= start) && (value <= end); }
inline uint8_t clib_between_f(int start, int end, int value) { return (value >= start) && (value <= end); }


#endif /* !CLIB_H */
