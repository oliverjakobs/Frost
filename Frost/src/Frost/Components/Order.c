#include "Order.h"

#include "..\FrostEcs.h"

ZIndex EntityGetZIndex(Ecs* ecs, EcsEntityID entity)
{
	ZIndex* z_index = EcsGetOrderComponent(ecs, entity, COMPONENT_Z_INDEX);
	return z_index ? *z_index : 0;
}

int ZIndexCmp(const ZIndex* left, const ZIndex* right)
{
	return *left - *right;
}
