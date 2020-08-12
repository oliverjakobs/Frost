#ifndef CLIB_LIST_H
#define CLIB_LIST_H

#include <stdlib.h>

typedef unsigned char clib_list_base_type;

typedef enum
{
    CLIB_LIST_OK = 0,
    CLIB_LIST_ALLOC_ERROR
} clib_list_error;

typedef struct
{
    clib_list_base_type* data;
    size_t capacity;
    size_t used;

    int (*cmp_func)(const void*,const void*);

    size_t element_size;
} clib_list;

/* Allocates a new list and initializes it */
clib_list_error clib_list_alloc(clib_list* list, size_t initial_size, size_t element_size, int (*cmp)(const void*, const void*));

/* Frees the list and all associated memory */
void clib_list_free(clib_list* list);

/* Resizes the list to new_size */
clib_list_error clib_list_resize(clib_list* list, size_t new_size);

/* Resizes list to free unused capacity */
clib_list_error clib_list_shrink_to_fit(clib_list* list);

/* Clears the array (lists list->used to 0) */
void clib_list_clear(clib_list* list);

/* Inserts an element at the end of the list if there is enough capacity */
void* clib_list_insert(clib_list* list, void* element);

/* Inserts an element at the end of the list and grows the list if necessary */
void* clib_list_insert_and_grow(clib_list* list, void* element, float growth);

/* Removes the element if the list cointains it */
void clib_list_remove(clib_list* list, const void* element);

/* Removes the element at the given index */
void clib_list_remove_at(clib_list* list, size_t index);

/* Searches the list with bsearch and returns the element if found or NULL else */
void* clib_list_find(clib_list* list, const void* element);

/* Searches the list with bsearch and returns the index of the element if found or list->used else */
size_t clib_list_find_index(clib_list* list, const void* element);

/* Returns the element at the given index */
void* clib_list_get(clib_list* list, size_t index); 

/* Returns the first element */
void* clib_list_first(clib_list* list);

/* Returns the last element */
void* clib_list_last(clib_list* list);

#endif /* CLIB_LIST_H */
