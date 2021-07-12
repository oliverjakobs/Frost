#ifndef ENTITY_ORDER_H
#define ENTITY_ORDER_H

#include "Scenes/Scene.h"

typedef struct
{
    const char* path;
} Template;

void TemplateLoad(Ecs* ecs, EcsEntityID entity, const char* templ);
void TemplateRelease(Template* templ);

typedef int ZIndex;

ZIndex EntityGetZIndex(Ecs* ecs, EcsEntityID entity);
int ZIndexCmp(const ZIndex* left, const ZIndex* right);

#endif /* !ENTITY_ORDER_H */