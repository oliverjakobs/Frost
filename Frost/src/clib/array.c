#include "array.h"

#include <string.h>

void clib_array_alloc(clib_array* arr, size_t initial_size, size_t element_size)
{
    arr->data = malloc(initial_size * element_size);
    arr->capacity = initial_size;
    arr->used = 0;

    arr->element_size = element_size;
}

void clib_array_resize(clib_array* arr, size_t new_size)
{
    clib_array_base_type* data = realloc(arr->data, new_size * arr->element_size);
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
    arr->capacity = 0;
    arr->used = 0;
    arr->element_size = 0;
}

void clib_array_clear(clib_array* arr)
{
    arr->used = 0;
}

void* clib_array_push(clib_array* arr, void* element)
{
    return clib_array_insert(arr, element, arr->used);
}

void* clib_array_push_and_grow(clib_array* arr, void* element, float growth)
{
    if (arr->used >= arr->capacity)
    {
        size_t size = (arr->capacity > 0) ? arr->capacity : 1;
        clib_array_resize(arr, (size_t)(size * growth));
    }

    return clib_array_push(arr, element);
}

void* clib_array_insert(clib_array* arr, void* element, size_t index)
{
    /* array is to small to insert */
    if (arr->used >= arr->capacity)
        return NULL;

    /* index is out of bounds (inserting would not result in a coherent array) */
    if (index > arr->used)
        return NULL;

    /* move entries back to make space for new element if index is not at the end */
    if (index < arr->used)
    {
        size_t size = (arr->used - index) * arr->element_size;
        size_t dest_offset = (index + 1) * arr->element_size;
        size_t src_offset = index * arr->element_size;

        memcpy(arr->data + dest_offset, arr->data + src_offset, size);
    }

    arr->used++;

    /* copy entry into the array */
    size_t offset = index * arr->element_size;
    return memcpy(arr->data + offset, element, arr->element_size);
}

void clib_array_remove(clib_array* arr, size_t index)
{
    if(index >= arr->used)
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

void* clib_array_last(clib_array* arr)
{
    return clib_array_get(arr, arr->used - 1);
}

void clib_array_sort(clib_array* arr, int (*cmp)(const void*, const void*))
{
    qsort(arr->data, arr->used, arr->element_size, cmp);
}

size_t clib_array_search(clib_array* arr, const void* element, int (*cmp)(const void*, const void*))
{
    clib_array_base_type* found = bsearch(element, arr->data, arr->used, arr->element_size, cmp);

    if (!found) return arr->used;

    return (found - arr->data) / arr->element_size;
}
