#include "Order.h"

#include "Frost/Frost.h"

void TemplateLoad(Ecs* ecs, EcsEntityID entity, const char* path, int variant)
{
    Template templ;
    templ.path = FrostMemDup(path, strlen(path) + 1);
    templ.variant = variant;
    EcsAddOrderComponent(ecs, entity, COMPONENT_TEMPLATE, &templ);
}

void TemplateRelease(Template* templ)
{
    FrostConstFree(templ->path);
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
