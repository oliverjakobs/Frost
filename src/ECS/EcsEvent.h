#ifndef ECS_EVENT_H
#define ECS_EVENT_H

#include "EcsCommon.h"

typedef uint32_t EcsEventType;
typedef void (*EcsEventCallback)(Ecs*, EcsEntityID, int);

void EcsEventSubscribe(EcsEventType type, EcsEventCallback callback);
void EcsEventThrow(Ecs* ecs, EcsEventType type, EcsEntityID id, int i);

#endif /* !ECS_EVENT_H */