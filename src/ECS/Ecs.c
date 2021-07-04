#include "Ecs.h"

#include <string.h>

void EcsInit(Ecs* ecs)
{
    ecs->update_systems = NULL;
    ecs->update_sys_cap = ecs->update_sys_len = 0;

    ecs->render_systems = NULL;
    ecs->render_sys_cap = ecs->render_sys_len = 0;

    ecs->data_components = NULL;
    ecs->data_comp_cap = ecs->data_comp_len = 0;

    ecs->order_components = NULL;
    ecs->order_comp_cap = ecs->order_comp_len = 0;

    memset(ecs->subscriptions, 0, ECS_MAX_EVENTS * sizeof(EcsEventCallback));
}

void EcsDestroy(Ecs* ecs)
{
    EcsMemFree(ecs->update_systems);
    ecs->update_sys_cap = ecs->update_sys_len = 0;

    EcsMemFree(ecs->render_systems);
    ecs->render_sys_cap = ecs->render_sys_len = 0;

    for (size_t i = 0; i < ecs->data_comp_len; ++i) EcsMapFree(&ecs->data_components[i]);
    EcsMemFree(ecs->data_components);
    ecs->data_comp_cap = ecs->data_comp_len = 0;

    for (size_t i = 0; i < ecs->order_comp_len; ++i) EcsListFree(&ecs->order_components[i]);
    EcsMemFree(ecs->order_components);
    ecs->order_comp_cap = ecs->order_comp_len = 0;
}

void EcsReserveSystems(Ecs* ecs, size_t update, size_t render)
{
    if (update > ecs->update_sys_cap)
    {
        ecs->update_systems = EcsMemMalloc(update * sizeof(EcsUpdateSystem));
        ecs->update_sys_cap = update;
    }

    if (update > ecs->render_sys_cap)
    {
        ecs->render_systems = EcsMemMalloc(render * sizeof(EcsRenderSystem));
        ecs->render_sys_cap = render;
    }
}

void EcsReserveComponents(Ecs* ecs, size_t data, size_t order)
{
    if (data > ecs->data_comp_cap)
    {
        ecs->data_components = EcsMemMalloc(data * sizeof(EcsMap));
        ecs->data_comp_cap = data;
    }

    if (order > ecs->order_comp_cap)
    {
        ecs->order_components = EcsMemMalloc(order * sizeof(EcsList));
        ecs->order_comp_cap = order;
    }
}

void EcsClear(Ecs* ecs)
{
    for (size_t i = 0; i < ecs->data_comp_len; ++i)  EcsMapClear(&ecs->data_components[i]);
    for (size_t i = 0; i < ecs->order_comp_len; ++i) EcsListClear(&ecs->order_components[i]);

    EcsEntityResetIDCounter();
}

void EcsAddUpdateSystem(Ecs* ecs, EcsUpdateCallback update)
{
    if (ecs->update_sys_len >= ecs->update_sys_cap)
        ecs->update_systems = EcsArrayGrow(ecs->update_systems, sizeof(EcsUpdateSystem), &ecs->update_sys_cap);

    if (ecs->update_systems) ecs->update_systems[ecs->update_sys_len++] = ((EcsUpdateSystem) { update });
}

void EcsAddRenderSystem(Ecs* ecs, EcsRenderStage stage, EcsRenderCallback render)
{
    if (ecs->render_sys_len >= ecs->render_sys_cap)
        ecs->render_systems = EcsArrayGrow(ecs->render_systems, sizeof(EcsRenderSystem), &ecs->render_sys_cap);

    if (ecs->render_systems) ecs->render_systems[ecs->render_sys_len++] = ((EcsRenderSystem) { stage, render });
}

void EcsOnUpdate(Ecs* ecs, void* world, float deltatime)
{
    for (size_t i = 0; i < ecs->update_sys_len; ++i)
        ecs->update_systems[i].update(ecs, world, deltatime);
}

void EcsOnRender(const Ecs* ecs, EcsRenderStage stage, const void* world, const float* mat_view_proj)
{
    for (size_t i = 0; i < ecs->render_sys_len; ++i)
        if (ecs->render_systems[i].stage == stage) ecs->render_systems[i].render(ecs, world, mat_view_proj);
}

void EcsRemoveEntity(Ecs* ecs, EcsEntityID entity)
{
    for (uint32_t i = 0; i < ecs->data_comp_len; ++i)  EcsRemoveDataComponent(ecs, entity, i);
    for (uint32_t i = 0; i < ecs->order_comp_len; ++i) EcsRemoveOrderComponent(ecs, entity, i);
}

EcsMap* EcsGetComponentMap(const Ecs* ecs, EcsComponentType type)
{
    return type < ecs->data_comp_len ? &ecs->data_components[type] : NULL;
}

EcsList* EcsGetComponentList(const Ecs* ecs, EcsComponentType type)
{
    return type < ecs->order_comp_len ? &ecs->order_components[type] : NULL;
}

int EcsRegisterDataComponent(Ecs* ecs, size_t size, size_t initial, EcsReleaseFunc release)
{
    EcsMap comp;
    if (!EcsMapAlloc(&comp, size, initial, release)) return 0;

    if (ecs->data_comp_len >= ecs->data_comp_cap)
        ecs->data_components = EcsArrayGrow(ecs->data_components, sizeof(EcsMap), &ecs->data_comp_cap);

    if (!ecs->data_components) return 0;

    ecs->data_components[ecs->data_comp_len++] = comp;
    return 1;
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

    if (ecs->order_comp_len >= ecs->order_comp_cap)
        ecs->order_components = EcsArrayGrow(ecs->order_components, sizeof(EcsList), &ecs->order_comp_cap);

    if (!ecs->order_components) return 0;

    ecs->order_components[ecs->order_comp_len++] = comp;
    return 1;
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