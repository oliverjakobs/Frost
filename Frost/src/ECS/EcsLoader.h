#ifndef ECS_LOADER_H
#define ECS_LOADER_H

#include "Ecs.h"

#define ECS_SIZEOF_LOADER(loader) (sizeof(loader) / sizeof((loader)[0])) 

typedef struct
{
    size_t element_size;
    EcsFreeFunc free;
} EcsDataComponentLoader;

void EcsLoadDataComponents(Ecs* ecs, EcsDataComponentLoader* loader, size_t size);

typedef struct
{
    size_t element_size;
    EcsFreeFunc free;
    EcsCmpFunc cmp;
} EcsOrderComponentLoader;

void EcsLoadOrderComponents(Ecs* ecs, EcsOrderComponentLoader* loader, size_t size);

void EcsLoadUpdateSystems(Ecs* ecs, EcsUpdateSystem* systems, size_t size);
void EcsLoadRenderSystems(Ecs* ecs, EcsRenderSystem* systems, size_t size);

#endif /* ECS_LOADER_H */
