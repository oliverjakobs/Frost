#ifndef ENTITY_ORDER_H
#define ENTITY_ORDER_H

#include "ECS/Ecs.h"

typedef struct
{
	char* templ;
} Template;

typedef int ZIndex;

ZIndex EntityGetZIndex(Ecs* ecs, EcsEntityID entity);
int ZIndexCmp(const ZIndex* left, const ZIndex* right);

#endif /* !ENTITY_ORDER_H */