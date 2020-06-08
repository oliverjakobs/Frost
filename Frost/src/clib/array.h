#ifndef CLIB_ARRAY_H
#define CLIB_ARRAY_H

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char clib_array_base_type;

typedef struct
{
    clib_array_base_type* data;
    size_t capacity;
    size_t used;

    size_t element_size;
} clib_array;

void clib_array_alloc(clib_array* arr, size_t initial_size, size_t element_size);
void clib_array_resize(clib_array* arr, size_t new_size);
void clib_array_shrink_to_fit(clib_array* arr);

void clib_array_free(clib_array* arr);

void clib_array_clear(clib_array* arr);

void* clib_array_insert(clib_array* arr, void* element);
void clib_array_remove(clib_array* arr, size_t index);

void* clib_array_get(clib_array* arr, size_t index); 
void* clib_array_get_next(clib_array* arr);

#endif /* !CLIB_ARRAY_H */
