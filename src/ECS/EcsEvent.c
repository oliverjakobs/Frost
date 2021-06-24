#include "EcsEvent.h"

#define ECS_MAX_EVENTS 8

EcsEventCallback _ecs_callbacks[ECS_MAX_EVENTS] = { 0 };

void EcsEventSubscribe(EcsEventType type, EcsEventCallback callback)
{
    if (type >= ECS_MAX_EVENTS) return;
    _ecs_callbacks[type] = callback;
}

void EcsEventThrow(Ecs* ecs, EcsEventType type, EcsEntityID id, int i)
{
    if (type >= ECS_MAX_EVENTS) return;
    if (_ecs_callbacks[type]) _ecs_callbacks[type](ecs, id, i);
}