#ifndef ECS_COMPONENT_LIST_H
#define ECS_COMPONENT_LIST_H

#include "Entity.h"

typedef struct
{
    char* data;
    size_t len;
    size_t cap;
    size_t element_size;

    int (*cmp_func)(const void*, const void*);
} EcsComponentList;

int EcsComponentListAlloc(EcsComponentList* list, size_t elem_size, int (*cmp)(const void*, const void*));
void EcsComponentListFree(EcsComponentList* list);

void EcsComponentListClear(EcsComponentList* list);

int EcsComponentListResize(EcsComponentList* list, size_t size);

void* EcsComponentListInsert(EcsComponentList* list, void* component);
void EcsComponentListRemove(EcsComponentList* list, size_t index);

void* EcsComponentListFind(const EcsComponentList* list, EcsEntityID entity);
void* EcsComponentListAt(const EcsComponentList* list, size_t index);

#endif /* !ECS_COMPONENT_LIST_H */
