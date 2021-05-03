#ifndef ECS_LIST_H
#define ECS_LIST_H

#include "EcsCommon.h"

typedef struct EcsListNode EcsListNode;

typedef struct
{
    EcsListNode* first;
    size_t element_size;

    EcsFreeFunc free;
    EcsCmpFunc cmp;
} EcsList;

int EcsListAlloc(EcsList* list, size_t elem_size, EcsFreeFunc free, EcsCmpFunc cmp);
void EcsListFree(EcsList* list);

void EcsListClear(EcsList* list);

void* EcsListInsert(EcsList* list, EcsEntityID entity, const void* component);
void EcsListRemove(EcsList* list, EcsEntityID entity);

void* EcsListFind(const EcsList* list, EcsEntityID entity);

EcsEntityID EcsListNodeEntity(const EcsListNode* node);
void* EcsListNodeComponent(const EcsListNode* node);
EcsListNode* EcsListNodeNext(const EcsListNode* node);

#endif /* !ECS_LIST_H */
