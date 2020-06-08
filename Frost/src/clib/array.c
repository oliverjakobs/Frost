#include "array.h"

#include <string.h>

void clib_array_alloc(clib_array* arr, size_t initial_size, size_t element_size)
{
    arr->data = (clib_array_base_type*)malloc(initial_size * element_size);
    arr->capacity = initial_size;
    arr->used = 0;

    arr->element_size = element_size;
}

void clib_array_resize(clib_array* arr, size_t new_size)
{
    clib_array_base_type* data = (clib_array_base_type*)realloc(arr->data, new_size * arr->element_size);
    if (data)
    {
        arr->data = data;
        arr->capacity = new_size;
    }
}

void clib_array_shrink_to_fit(clib_array* arr)
{
    clib_array_resize(arr, arr->used);
}

void clib_array_free(clib_array* arr)
{
    free(arr->data);
    clib_array_clear(arr);
    arr->element_size = 0;
    arr->capacity = 0;
}

void clib_array_clear(clib_array* arr)
{
    arr->used = 0;
}

void* clib_array_insert(clib_array* arr, void* element)
{
    if (arr->used >= arr->capacity)
        return NULL;

    size_t offset = arr->used++ * arr->element_size;
    return memcpy(arr->data + offset, element, arr->element_size);
}

void clib_array_remove(clib_array* arr, size_t index)
{
    if (index >= arr->used)
        return;

    size_t size = (arr->used - (index + 1)) * arr->element_size;
    size_t dest_offset = index * arr->element_size;
    size_t src_offset = (index + 1) * arr->element_size;

    memcpy(arr->data + dest_offset, arr->data + src_offset, size);

    arr->used--;
}

void* clib_array_get(clib_array* arr, size_t index)
{
    if (index >= arr->used)
        return NULL;

    return arr->data + index * arr->element_size;
}

void* clib_array_get_next(clib_array* arr)
{
    if (arr->used >= arr->capacity)
        return NULL;

    return arr->data + arr->used++ * arr->element_size;
}
