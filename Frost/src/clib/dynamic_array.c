#include <stdio.h>
#include <stdlib.h>

#include "dynamic_array.h"

int clib_dynamic_array_init(clib_dynamic_array* v, size_t initial_capacity)
{
    v->capacity = initial_capacity;
    v->size = 0;
    v->elements = malloc(sizeof(void*) * v->capacity);

    if (v->elements) return 1;
    return 0;
}

void clib_dynamic_array_free(clib_dynamic_array* v)
{
    v->capacity = 0;
    v->size = 0;

    if (v->elements)
    {
        free(v->elements);
        v->elements = NULL;
    }
}

static void _clib_dynamic_array_resize(clib_dynamic_array *v, size_t capacity)
{
    void** elements = realloc(v->elements, sizeof(void*) * capacity);
    if (elements) 
    {
        v->elements = elements;
        v->capacity = capacity;
    }
}

void clib_dynamic_array_push(clib_dynamic_array* v, void* item)
{
    if (v->capacity == v->size)
        _clib_dynamic_array_resize(v, v->capacity * 2);

    v->elements[v->size++] = item;
}

void clib_dynamic_array_set(clib_dynamic_array* v, size_t index, void* item)
{
    if (index >= 0 && index < v->size)
        v->elements[index] = item;
}

void clib_dynamic_array_delete(clib_dynamic_array* v, size_t index)
{
    if (index < 0 || index >= v->size)
        return;

    v->elements[index] = NULL;

    for (size_t i = index; i < v->size - 1; i++)
    {
        v->elements[i] = v->elements[i + 1];
        v->elements[i + 1] = NULL;
    }

    v->size--;

    if (v->size > 0 && v->size == v->capacity / 4)
        _clib_dynamic_array_resize(v, v->capacity / 2);
}

void* clib_dynamic_array_get(const clib_dynamic_array* v, size_t index)
{
    if (index >= 0 && index < v->size)
        return v->elements[index];

    return NULL;
}
