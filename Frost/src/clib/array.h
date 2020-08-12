#ifndef CLIB_ARRAY_H
#define CLIB_ARRAY_H

#include <stdlib.h>

typedef unsigned char clib_array_base_type;

typedef enum
{
    CLIB_ARRAY_OK = 0,
    CLIB_ARRAY_ALLOC_ERROR
} clib_array_error;

typedef struct
{
    clib_array_base_type* data;
    size_t capacity;
    size_t used;

    size_t element_size;
} clib_array;

/* Allocates a new array and initializes it */
clib_array_error clib_array_alloc(clib_array* arr, size_t initial_size, size_t element_size);

/* Frees the array and all associated memory */
void clib_array_free(clib_array* arr);

/* Resizes the set to new_size */
clib_array_error clib_array_resize(clib_array* arr, size_t new_size);

/* Resizes array to free unused capacity */
clib_array_error clib_array_shrink_to_fit(clib_array* arr);

/* Clears the array (sets arr->used to 0) */
void clib_array_clear(clib_array* arr);

/* Inserts an element at the end of the array if there is enough capacity */
void* clib_array_push(clib_array* arr, void* element);

/* Inserts an element at the end of the array and grows the array if necessary */
void* clib_array_push_and_grow(clib_array* arr, void* element, float growth);

/* Inserts an element at the given index if the index is not out of bounds */
void* clib_array_insert(clib_array* arr, void* element, size_t index);

/* Removes the element at the given index */
void clib_array_remove(clib_array* arr, size_t index);

/* Returns the element at the given index */
void* clib_array_get(clib_array* arr, size_t index); 

/* Returns the first element */
void* clib_array_first(clib_array* arr);

/* Returns the last element */
void* clib_array_last(clib_array* arr);

/* sort array with qsort */
void clib_array_sort(clib_array* arr, int (*cmp)(const void*, const void*));

/* Searches the array with bsearch and returns the index of the element if found or arr->used else */
size_t clib_array_search(clib_array* arr, const void* element, int (*cmp)(const void*, const void*));

#endif /* !CLIB_ARRAY_H */
