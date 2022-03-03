#include "Ecs.h"

#include <string.h>

void EcsInit(Ecs* ecs)
{
    ecs->update.entries = NULL;
    ecs->update.cap = ecs->update.len = 0;

    ecs->render.entries = NULL;
    ecs->render.cap = ecs->render.len = 0;

    ecs->data.entries = NULL;
    ecs->data.cap = ecs->data.len = 0;

    ecs->order.entries = NULL;
    ecs->order.cap = ecs->order.len = 0;

    memset(ecs->subscriptions, 0, ECS_MAX_EVENTS * sizeof(EcsEventCallback));
}

void EcsDestroy(Ecs* ecs)
{
    EcsMemFree(ecs->update.entries);
    ecs->update.cap = ecs->update.len = 0;

    EcsMemFree(ecs->render.entries);
    ecs->render.cap = ecs->render.len = 0;

    for (size_t i = 0; i < ecs->data.len; ++i) EcsMapFree(&ecs->data.entries[i]);
    EcsMemFree(ecs->data.entries);
    ecs->data.cap = ecs->data.len = 0;

    for (size_t i = 0; i < ecs->order.len; ++i) EcsListFree(&ecs->order.entries[i]);
    EcsMemFree(ecs->order.entries);
    ecs->order.cap = ecs->order.len = 0;
}

void EcsReserveSystems(Ecs* ecs, size_t update, size_t render)
{
    if (update > ecs->update.cap)
    {
        ecs->update.entries = EcsMemMalloc(update * sizeof(EcsUpdateSystem));
        ecs->update.cap = update;
    }

    if (update > ecs->render.cap)
    {
        ecs->render.entries = EcsMemMalloc(render * sizeof(EcsRenderSystem));
        ecs->render.cap = render;
    }
}

void EcsReserveComponents(Ecs* ecs, size_t data, size_t order)
{
    if (data > ecs->data.cap)
    {
        ecs->data.entries = EcsMemMalloc(data * sizeof(EcsMap));
        ecs->data.cap = data;
    }

    if (order > ecs->order.cap)
    {
        ecs->order.entries = EcsMemMalloc(order * sizeof(EcsList));
        ecs->order.cap = order;
    }
}

void EcsClear(Ecs* ecs)
{
    for (size_t i = 0; i < ecs->data.len; ++i)  EcsMapClear(&ecs->data.entries[i]);
    for (size_t i = 0; i < ecs->order.len; ++i) EcsListClear(&ecs->order.entries[i]);

    EcsEntityResetIDCounter();
}

static size_t EcsArrayPush(void** arr, size_t element_size, size_t cap, size_t len)
{
    return (len >= cap) ? EcsMemArrayGrow(arr, element_size, cap) : cap;
}

void EcsAddUpdateSystem(Ecs* ecs, EcsUpdateCallback update)
{
    ecs->update.cap = EcsArrayPush(&ecs->update.entries, sizeof(EcsUpdateSystem), ecs->update.cap, ecs->update.len);
    if (ecs->update.entries) ecs->update.entries[ecs->update.len++] = ((EcsUpdateSystem) { update });
}

void EcsAddRenderSystem(Ecs* ecs, EcsRenderStage stage, EcsRenderCallback render)
{
    ecs->render.cap = EcsArrayPush(&ecs->render.entries, sizeof(EcsUpdateSystem), ecs->render.cap, ecs->render.len);
    if (ecs->render.entries) ecs->render.entries[ecs->render.len++] = ((EcsRenderSystem) { stage, render });
}

void EcsOnUpdate(Ecs* ecs, void* world, float deltatime)
{
    for (size_t i = 0; i < ecs->update.len; ++i)
        ecs->update.entries[i].update(ecs, world, deltatime);
}

void EcsOnRender(const Ecs* ecs, EcsRenderStage stage, const void* world, const float* mat_view_proj)
{
    for (size_t i = 0; i < ecs->render.len; ++i)
        if (ecs->render.entries[i].stage == stage) ecs->render.entries[i].render(ecs, world, mat_view_proj);
}

void EcsRemoveEntity(Ecs* ecs, EcsEntityID entity)
{
    for (uint32_t i = 0; i < ecs->data.len; ++i)  EcsRemoveDataComponent(ecs, entity, i);
    for (uint32_t i = 0; i < ecs->order.len; ++i) EcsRemoveOrderComponent(ecs, entity, i);
}

EcsMap* EcsGetComponentMap(const Ecs* ecs, EcsComponentType type)
{
    return type < ecs->data.len ? &ecs->data.entries[type] : NULL;
}

EcsList* EcsGetComponentList(const Ecs* ecs, EcsComponentType type)
{
    return type < ecs->order.len ? &ecs->order.entries[type] : NULL;
}

int EcsRegisterDataComponent(Ecs* ecs, size_t size, size_t initial, EcsReleaseFunc release)
{
    EcsMap comp;
    if (!EcsMapAlloc(&comp, size, initial, release)) return 0;

    ecs->data.cap = EcsArrayPush(&ecs->data.entries, sizeof(EcsMap), ecs->data.cap, ecs->data.len);
    if (ecs->data.entries) ecs->data.entries[ecs->data.len++] = comp;

    return ecs->data.cap > 0;
}

void* EcsAddDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type, const void* component)
{
    return EcsMapInsert(EcsGetComponentMap(ecs, type), entity, component);
}

void* EcsGetDataComponent(const Ecs* ecs, EcsEntityID entity, EcsComponentType type)
{
    return EcsMapFind(EcsGetComponentMap(ecs, type), entity);
}

void EcsRemoveDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type)
{
    EcsMapRemove(EcsGetComponentMap(ecs, type), entity);
}

int EcsRegisterOrderComponent(Ecs* ecs, size_t size, size_t initial, EcsReleaseFunc release, EcsCmpFunc cmp)
{
    EcsList comp;
    if (!EcsListAlloc(&comp, size, initial, release, cmp)) return 0;

    ecs->order.cap = EcsArrayPush(&ecs->order.entries, sizeof(EcsList), ecs->order.cap, ecs->order.len);
    if (ecs->order.entries) ecs->order.entries[ecs->order.len++] = comp;

    return ecs->order.cap > 0;
}

void* EcsAddOrderComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type, const void* component)
{
    return EcsListInsert(EcsGetComponentList(ecs, type), entity, component);
}

void* EcsGetOrderComponent(const Ecs* ecs, EcsEntityID entity, EcsComponentType type)
{
    return EcsListFind(EcsGetComponentList(ecs, type), entity);
}

void EcsRemoveOrderComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type)
{
    EcsListRemove(EcsGetComponentList(ecs, type), entity);
}

void EcsEventSubscribe(Ecs* ecs, EcsEventType type, EcsEventCallback callback)
{
    if (type >= ECS_MAX_EVENTS) return;
    ecs->subscriptions[type] = callback;
}

void EcsEventThrow(Ecs* ecs, EcsEventType type, EcsEntityID id, int i)
{
    if (type >= ECS_MAX_EVENTS) return;
    if (ecs->subscriptions[type]) ecs->subscriptions[type](ecs, id, i);
}