#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Scenes/Scene.h"

/*
TEMPLATE:
[transform]
width = <float : 0.0>
height = <float : 0.0>
*/

typedef struct
{
    vec2 position;
    vec2 size;
} Transform;

void TransformLoad(char* ini, Ecs* ecs, EcsEntityID entity, vec2 pos);

EcsEntityID GetEntityAt(const Ecs* ecs, vec2 pos);

void SetEntityPosition(const Ecs* ecs, EcsEntityID entity, vec2 pos);

vec2 GetEntityPosition(const Ecs* ecs, EcsEntityID entity);
vec2 GetEntityCenter(const Ecs* ecs, EcsEntityID entity);

rect GetEntityRect(const Ecs* ecs, EcsEntityID entity);

#endif /* !TRANSFORM_H */