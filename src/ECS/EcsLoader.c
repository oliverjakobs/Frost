#include "EcsLoader.h"

void EcsLoadDataComponents(Ecs* ecs, EcsDataComponentLoader* loader, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        EcsDataComponentLoader* comp = &loader[i];
        if (comp) EcsRegisterDataComponent(ecs, comp->element_size, comp->release);
    }
}

void EcsLoadOrderComponents(Ecs* ecs, EcsOrderComponentLoader* loader, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        EcsOrderComponentLoader* comp = &loader[i];
        if (comp) EcsRegisterOrderComponent(ecs, comp->element_size, comp->release, comp->cmp);
    }
}

void EcsLoadUpdateSystems(Ecs* ecs, EcsUpdateSystem* systems, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        EcsUpdateSystem* sys = &systems[i];
        if (sys) EcsAddUpdateSystem(ecs, sys->update);
    }
}

void EcsLoadRenderSystems(Ecs* ecs, EcsRenderSystem* systems, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        EcsRenderSystem* sys = &systems[i];
        if (sys) EcsAddRenderSystem(ecs, sys->stage, sys->render);
    }
}
