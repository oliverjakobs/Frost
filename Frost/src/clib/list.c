#include "list.h"

#include <string.h>
#include <stdio.h>

clib_list_error clib_list_alloc(clib_list* list, size_t initial_size, size_t element_size, int (*cmp)(const void*, const void*))
{
    list->data = malloc(initial_size * element_size);

    if (!list->data) return CLIB_LIST_ALLOC_ERROR;

    list->capacity = initial_size;
    list->used = 0;

    list->element_size = element_size;

    list->cmp_func = cmp;

    return CLIB_LIST_OK;
}

void clib_list_free(clib_list* list)
{
    free(list->data);
    list->capacity = 0;
    list->used = 0;
    list->element_size = 0;

    list->cmp_func = NULL;
}

clib_list_error clib_list_resize(clib_list* list, size_t new_size)
{
    clib_list_base_type* data = realloc(list->data, new_size * list->element_size);

    if (!data) return CLIB_LIST_ALLOC_ERROR;

    list->data = data;
    list->capacity = new_size;
    return CLIB_LIST_OK;
}

clib_list_error clib_list_shrink_to_fit(clib_list* list)
{
    return clib_list_resize(list, list->used);
}

void clib_list_clear(clib_list* list)
{
    list->used = 0;
}

void* clib_list_insert(clib_list* list, void* element)
{
    /* list is full */
    if (list->used >= list->capacity)
        return NULL;

    size_t index = 0;

    /* find location for the new element */
    while (index < list->used && list->cmp_func(clib_list_get(list, index), element) < 0)
        ++index;

    /* move entries back to make space for new element if index is not at the end */
    if (index < list->used)
    {
        size_t size = (list->used - index) * list->element_size;
        size_t dest_offset = (index + 1) * list->element_size;
        size_t src_offset = index * list->element_size;

        memcpy(list->data + dest_offset, list->data + src_offset, size);
    }
    
    list->used++;

    /* copy element into the list */
    size_t offset = index * list->element_size;
    return memcpy(list->data + offset, element, list->element_size);
}

void* clib_list_insert_and_grow(clib_list* list, void* element, float growth)
{
    if (list->used >= list->capacity)
    {
        size_t size = (list->capacity > 0) ? list->capacity : 1;
        if (clib_list_resize(list, (size_t)(size * growth)) != CLIB_LIST_OK)
            return NULL;
    }

    return clib_list_insert(list, element);
}

void clib_list_remove(clib_list* list, const void* element)
{
    clib_list_remove_at(list, clib_list_find_index(list, element));
}

void clib_list_remove_at(clib_list* list, size_t index)
{
    if(index >= list->used) return;
    
    size_t size = (list->used - (index + 1)) * list->element_size;
    size_t dest_offset = index * list->element_size;
    size_t src_offset = (index + 1) * list->element_size;

    memcpy(list->data + dest_offset, list->data + src_offset, size);

    list->used--;
}

void* clib_list_find(clib_list* list, const void* element)
{
    return bsearch(element, list->data, list->used, list->element_size, list->cmp_func);
}

size_t clib_list_find_index(clib_list* list, const void* element)
{
    clib_list_base_type* found = clib_list_find(list, element);

    if (!found) return list->used;

    return (found - list->data) / list->element_size;
}

void* clib_list_get(clib_list* list, size_t index)
{
    if (index >= list->used) return NULL;

    return list->data + index * list->element_size;
}

void* clib_list_first(clib_list* list)
{
    return clib_list_get(list, 0);
}

void* clib_list_last(clib_list* list)
{
    return clib_list_get(list, list->used - 1);
}

size_t clib_list_search(clib_list* list, const void* element)
{
    clib_list_base_type* found = bsearch(element, list->data, list->used, list->element_size, list->cmp_func);

    if (!found) return list->used;

    return (found - list->data) / list->element_size;
}