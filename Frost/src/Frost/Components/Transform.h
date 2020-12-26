#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "math/math.h"

#include "Scenes/Scene.h"

typedef struct
{
	vec2 position;
	vec2 size;
} Transform;

void TransformLoad(Scene* scene, EcsEntityID entity, vec2 pos, char* json);

EcsEntityID GetEntityAt(Ecs* ecs, vec2 pos);

void SetEntityPosition(Ecs* ecs, EcsEntityID entity, vec2 pos);

vec2 GetEntityPosition(Ecs* ecs, EcsEntityID entity);
vec2 GetEntityCenter(Ecs* ecs, EcsEntityID entity);

rect GetEntityRect(Ecs* ecs, EcsEntityID entity);

#endif /* !TRANSFORM_H */