#ifndef ITEM_H
#define ITEM_H

#include "Scenes/Scene.h"

/*
TEMPLATE:
[item]
id = <int | VARIANT : NULL_ITEM> 
*/

#define NULL_ITEM (-1)
typedef int32_t ItemID;

typedef struct
{
    ItemID id;
} Item;

void ItemLoad(char* ini, Scene* scene, EcsEntityID entity, int variant);

#endif /* !ITEM_H */