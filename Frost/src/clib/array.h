#ifndef CLIB_ARRAY_H
#define CLIB_ARRAY_H

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

/* Resizes array to free unused capacity */
void clib_array_shrink_to_fit(clib_array* arr);

/* Frees the array */
void clib_array_free(clib_array* arr);

void clib_array_clear(clib_array* arr);

/* Inserts an element at the end of the array if there is enough capaxity */
void* clib_array_push(clib_array* arr, void* element);

/* Inserts an element at the end of the array and resizes the array if necessary */
void* clib_array_push_and_grow(clib_array* arr, void* element, float growth);

void* clib_array_insert(clib_array* arr, void* element, size_t index);

void clib_array_remove(clib_array* arr, size_t index);

void* clib_array_get(clib_array* arr, size_t index); 
void* clib_array_last(clib_array* arr);

/* sort array with qsort */
void clib_array_sort(clib_array* arr, int (*cmp)(const void*, const void*));

#endif /* !CLIB_ARRAY_H */
