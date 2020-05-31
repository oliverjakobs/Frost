#ifndef CLIB_DYNAMIC_ARRAY_H
#define CLIB_DYNAMIC_ARRAY_H

#include <stddef.h>

#define CLIB_DYNAMIC_ARRAY_DECLARE_FUNCS(name, type)                                \
    void name##_dynamic_array_push(clib_dynamic_array* v, type* item);              \
    void name##_dynamic_array_set(clib_dynamic_array* v, size_t index, type* item); \
    void name##_dynamic_array_delete(clib_dynamic_array* v, size_t index);          \
    type* name##_dynamic_array_get(const clib_dynamic_array* v, size_t index);

#define CLIB_DYNAMIC_ARRAY_DEFINE_FUNCS(name, type)                                 \
    void name##_dynamic_array_push(clib_dynamic_array* v, type* item)               \
    {                                                                               \
        clib_dynamic_array_push(v, (void*)item);                                    \
    }                                                                               \
    void name##_dynamic_array_set(clib_dynamic_array* v, size_t index, type* item)  \
    {                                                                               \
        clib_dynamic_array_set(v, index, (void*)item);                              \
    }                                                                               \
    void name##_dynamic_array_delete(clib_dynamic_array* v, size_t index)           \
    {                                                                               \
        clib_dynamic_array_delete(v, index);                                        \
    }                                                                               \
    type* name##_dynamic_array_get(const clib_dynamic_array* v, size_t index)       \
    {                                                                               \
        return (type*)clib_dynamic_array_get(v, index);                             \
    }


typedef struct
{
    void** elements;
    size_t capacity;
    size_t size;
} clib_dynamic_array;

int clib_dynamic_array_init(clib_dynamic_array* v, size_t initial_capacity);
void clib_dynamic_array_free(clib_dynamic_array* v);

void clib_dynamic_array_push(clib_dynamic_array* v, void* item);
void clib_dynamic_array_set(clib_dynamic_array* v, size_t index, void* item);
void clib_dynamic_array_delete(clib_dynamic_array* v, size_t index);

void* clib_dynamic_array_get(const clib_dynamic_array* v, size_t index);

#endif /* !CLIB_DYNAMIC_ARRAY_H */