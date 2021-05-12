#ifndef ECS_COMMON_H
#define ECS_COMMON_H

#include <stdint.h>

typedef int32_t EcsEntityID;
#define ECS_NULL_ENTITY (0)

void EcsEntityResetIDCounter();
EcsEntityID EcsEntityGetNextID();

typedef void (*EcsReleaseFunc)(void*);
typedef int (*EcsCmpFunc)(const void*, const void*);

typedef uint32_t EcsComponentType;
typedef struct
{
	EcsEntityID entity;
	void* data;
} EcsEntry;

int EcsEntryFill(EcsEntry* comp, EcsEntityID entity, const void* data, size_t size);
void EcsEntryClear(EcsEntry* comp, EcsReleaseFunc release);

int EcsEntryCmpID(const EcsEntry* left, const EcsEntry* right);
int EcsEntryCmp(const EcsEntry* left, const EcsEntry* right, EcsCmpFunc cmp);

typedef struct Ecs Ecs;

typedef void (*EcsUpdateCallback)(Ecs*, void*, float);
typedef void (*EcsRenderCallback)(const Ecs*, const void*, const float*);

/* Memory */
void* EcsMemAlloc(size_t size);
void* EcsMemRealloc(void* block, size_t size);
void* EcsMemDup(const void* block, size_t size);
void EcsMemFree(void* block);

#endif /* !ECS_COMMON_H */
