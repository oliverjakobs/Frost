#ifndef ENTITY_ORDER_H
#define ENTITY_ORDER_H

#include "Scenes/Scene.h"

typedef struct
{
	char* templ;
} Template;

void TemplateLoad(Scene* scene, EcsEntityID entity, const char* templ);
void TemplateFree(Template* templ);

typedef int ZIndex;

ZIndex EntityGetZIndex(Ecs* ecs, EcsEntityID entity);
int ZIndexCmp(const ZIndex* left, const ZIndex* right);

#endif /* !ENTITY_ORDER_H */