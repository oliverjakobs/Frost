#include "Loader.h"

void EcsLoadDataComponents(Ecs* ecs, EcsDataComponentLoader* loader, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        EcsDataComponentLoader* comp = &loader[i];
        if (comp) EcsRegisterDataComponent(ecs, comp->element_size, comp->free_func);
    }
}

void EcsLoadOrderComponents(Ecs* ecs, EcsOrderComponentLoader* loader, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        EcsOrderComponentLoader* comp = &loader[i];
        if (comp) EcsRegisterOrderComponent(ecs, comp->element_size, comp->free_func, comp->cmp_func);
    }
}

void EcsLoadUpdateSystems(Ecs* ecs, EcsUpdateSystemLoader* loader, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        EcsUpdateSystemLoader* sys = &loader[i];
        if (sys) EcsAddUpdateSystem(ecs, sys->update);
    }
}

void EcsLoadRenderSystems(Ecs* ecs, EcsRenderSystemLoader* loader, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        EcsRenderSystemLoader* sys = &loader[i];
        if (sys) EcsAddRenderSystem(ecs, sys->stage, sys->render);
    }
}
