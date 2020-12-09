#ifndef ECS_COMPONENT_LIST_H
#define ECS_COMPONENT_LIST_H

#include "Entity.h"

typedef struct EcsComponentNode EcsComponentNode;

struct EcsComponentNode
{
    EcsEntityID entity;
    void* component;
    EcsComponentNode* next;
};

typedef struct
{
    EcsComponentNode* first;
    size_t element_size;

    int (*cmp_func)(const void*, const void*);
} EcsComponentList;

int EcsComponentListAlloc(EcsComponentList* list, size_t elem_size, int (*cmp)(const void*, const void*));
void EcsComponentListFree(EcsComponentList* list);

void EcsComponentListClear(EcsComponentList* list);

void* EcsComponentListInsert(EcsComponentList* list, EcsEntityID entity, void* component);
void EcsComponentListRemove(EcsComponentList* list, EcsEntityID entity);

void* EcsComponentListFind(const EcsComponentList* list, EcsEntityID entity);

EcsEntityID EcsComponentNodeEntity(const EcsComponentNode* node);
void* EcsComponentNodeComponent(const EcsComponentNode* node);
EcsComponentNode* EcsComponentNodeNext(const EcsComponentNode* node);

#endif /* !ECS_COMPONENT_LIST_H */
