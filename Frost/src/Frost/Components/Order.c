#include "Order.h"

#include "..\FrostEcs.h"

void TemplateLoad(Ecs* ecs, EcsEntityID entity, const char* templ)
{
	Template t;
	t.templ = malloc(strlen(templ) + 1);
	strcpy(t.templ, templ);

	EcsAddOrderComponent(ecs, entity, COMPONENT_TEMPLATE, &t);
}

void TemplateFree(Template* templ)
{
	free(templ->templ);
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
