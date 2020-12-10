#ifndef ECS_COMPONENT_LIST_H
#define ECS_COMPONENT_LIST_H

#include "Entity.h"

typedef struct EcsListNode EcsListNode;

typedef struct
{
    EcsListNode* first;
    size_t element_size;

    int (*cmp_func)(const void*, const void*);
} EcsComponentList;

int EcsComponentListAlloc(EcsComponentList* list, size_t elem_size, int (*cmp)(const void*, const void*));
void EcsComponentListFree(EcsComponentList* list);

void EcsComponentListClear(EcsComponentList* list);

void* EcsComponentListInsert(EcsComponentList* list, EcsEntityID entity, void* component);
void EcsComponentListRemove(EcsComponentList* list, EcsEntityID entity);

void* EcsComponentListFind(const EcsComponentList* list, EcsEntityID entity);

EcsEntityID EcsComponentNodeEntity(const EcsListNode* node);
void* EcsComponentNodeComponent(const EcsListNode* node);
EcsListNode* EcsComponentNodeNext(const EcsListNode* node);

#endif /* !ECS_COMPONENT_LIST_H */
