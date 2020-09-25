#ifndef FROST_ECS_H
#define FROST_ECS_H

#include "EcsLoader.h"

/* ----------------------| UTILITY |------------------------------ */
EcsEntityID GetEntityAt(Ecs* ecs, vec2 pos);

void SetEntityPosition(Ecs* ecs, EcsEntityID entity, vec2 pos);

vec2 GetEntityPosition(Ecs* ecs, EcsEntityID entity);
vec2 GetEntityCenter(Ecs* ecs, EcsEntityID entity);

rect GetEntityRect(Ecs* ecs, EcsEntityID entity);

#endif /* !FROST_ECS_H */
