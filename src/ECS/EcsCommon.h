#ifndef ECS_COMMON_H
#define ECS_COMMON_H

#include <stdint.h>

typedef int32_t EcsEntityID;
#define ECS_NULL_ENTITY (-1)

void EcsEntityResetIDCounter();
EcsEntityID EcsEntityGetNextID();

typedef uint32_t EcsComponentType;

typedef struct Ecs Ecs;

typedef void (*EcsUpdateCallback)(Ecs*, void*, float);
typedef void (*EcsRenderCallback)(const Ecs*, const void*, const float*);

typedef void (*EcsFreeFunc)(void*);
typedef int (*EcsCmpFunc)(const void*, const void*);

/* Memory */
void* EcsMemAlloc(size_t size);
void* EcsMemRealloc(void* block, size_t size);
void* EcsMemDup(const void* block, size_t size);
void EcsMemFree(void* block);

#endif /* !ECS_COMMON_H */
