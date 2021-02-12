#include "Order.h"

#include "../FrostEcs.h"

#include "toolbox/tb_str.h"

void TemplateLoad(Ecs* ecs, EcsEntityID entity, const char* templ)
{
	const char* t = tb_strdup(templ);
	EcsAddOrderComponent(ecs, entity, COMPONENT_TEMPLATE, &t);
}

ZIndex EntityGetZIndex(Ecs* ecs, EcsEntityID entity)
{
	ZIndex* z_index = EcsGetOrderComponent(ecs, entity, COMPONENT_Z_INDEX);
	return z_index ? *z_index : 0;
}

int ZIndexCmp(const ZIndex* left, const ZIndex* right)
{
	return *left - *right;
}
