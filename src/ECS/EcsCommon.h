#ifndef ECS_COMMON_H
#define ECS_COMMON_H

#include <stdint.h>

typedef int32_t EcsEntityID;
#define ECS_NULL_ENTITY (0)

void EcsEntityResetIDCounter();
void EcsEntityUseID(EcsEntityID id);
EcsEntityID EcsEntityGetNextID();

typedef void (*EcsReleaseFunc)(void*);
typedef int  (*EcsCmpFunc)(const void*, const void*);

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

typedef uint32_t EcsEventType;
typedef void (*EcsEventCallback)(Ecs*, EcsEntityID, int);

typedef void (*EcsUpdateCallback)(Ecs*, void*, float);
typedef void (*EcsRenderCallback)(const Ecs*, const void*, const float*);

/* Memory */
typedef void* (*EcsMallocCallback) (void* allocator, size_t size);
typedef void* (*EcsReallocCallback)(void* allocator, void* block, size_t size);
typedef void  (*EcsFreeCallback)   (void* allocator, void* block);

void EcsMemSetAllocator(void* allocator, EcsMallocCallback malloc, EcsReallocCallback realloc, EcsFreeCallback free);

void* EcsMemMalloc(size_t size);
void* EcsMemRealloc(void* block, size_t size);
void  EcsMemFree(void* block);

void* EcsMemCalloc(size_t count, size_t size);
void* EcsMemDup(const void* block, size_t size);

#define ECS_ARRAY_GROWTH 2.0

size_t EcsMemArrayGrow(void** arr, size_t elem_size, size_t cap);

#endif /* !ECS_COMMON_H */
