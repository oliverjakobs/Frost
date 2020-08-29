#ifndef ENTITY_ORDER_H
#define ENTITY_ORDER_H

#include "Ecs/Entity.h"

typedef struct
{
	EntityID entity;
	char* templ;
} Template;

int TemplateCmp(const Template* left, const Template* right);

typedef struct
{
	EntityID entity;
	int z_index;
} ZIndex;

int ZIndexCmp(const ZIndex* left, const ZIndex* right);

#endif /* !ENTITY_ORDER_H */