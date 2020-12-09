#ifndef ENTITY_ORDER_H
#define ENTITY_ORDER_H

#include "Ecs/Entity.h"

typedef struct
{
	char* templ;
} Template;

typedef struct
{
	int z_index;
} ZIndex;

int ZIndexCmp(const ZIndex* left, const ZIndex* right);

#endif /* !ENTITY_ORDER_H */