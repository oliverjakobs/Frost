#ifndef ECS_LOADER_H
#define ECS_LOADER_H

#include "Ecs.h"

#define ECS_SIZEOF_LOADER(loader) (sizeof(loader) / sizeof((loader)[0])) 

typedef struct
{
    size_t initial;
    size_t size;
    EcsReleaseFunc release;
} EcsDataComponentLoader;

void EcsLoadDataComponents(Ecs* ecs, EcsDataComponentLoader* loader, size_t size);

typedef struct
{
    size_t initial;
    size_t size;
    EcsReleaseFunc release;
    EcsCmpFunc cmp;
} EcsOrderComponentLoader;

void EcsLoadOrderComponents(Ecs* ecs, EcsOrderComponentLoader* loader, size_t size);

void EcsLoadUpdateSystems(Ecs* ecs, EcsUpdateSystem* systems, size_t size);
void EcsLoadRenderSystems(Ecs* ecs, EcsRenderSystem* systems, size_t size);

#endif /* ECS_LOADER_H */
