#ifndef ITEM_H
#define ITEM_H

#include "Scenes/Scene.h"

#define NULL_ITEM (-1)
typedef int32_t ItemID;

typedef struct
{
	ItemID id;
} Item;

void ItemLoad(char* json, Ecs* ecs, EcsEntityID entity, int variant);

#endif /* !ITEM_H */