#ifndef ECS_LOADER_H
#define ECS_LOADER_H

#include "Ecs.h"

#define ECS_SIZEOF_LOADER(loader) (sizeof(loader) / sizeof((loader)[0])) 

typedef struct
{
    size_t element_size;
    void (*free_func)(void*);
} EcsDataComponentLoader;

void EcsLoadDataComponents(Ecs* ecs, EcsDataComponentLoader* loader, size_t size);

typedef struct
{
    size_t element_size;
    void (*free_func)(void*);
    int (*cmp_func)(const void*, const void*);
} EcsOrderComponentLoader;

void EcsLoadOrderComponents(Ecs* ecs, EcsOrderComponentLoader* loader, size_t size);

void EcsLoadUpdateSystems(Ecs* ecs, EcsUpdateSystem* systems, size_t size);
void EcsLoadRenderSystems(Ecs* ecs, EcsRenderSystem* systems, size_t size);

#endif /* ECS_LOADER_H */
