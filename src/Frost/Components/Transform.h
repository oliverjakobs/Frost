#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Scenes/Scene.h"

/*
JSON - Template

"transform": {
	"size": [ <WIDTH:float>, <HEIGHT:float> ]
}
*/

typedef struct
{
	vec2 position;
	vec2 size;
} Transform;

void TransformLoad(char* json, Ecs* ecs, EcsEntityID entity, vec2 pos);

EcsEntityID GetEntityAt(const Ecs* ecs, vec2 pos);

void SetEntityPosition(const Ecs* ecs, EcsEntityID entity, vec2 pos);

vec2 GetEntityPosition(const Ecs* ecs, EcsEntityID entity);
vec2 GetEntityCenter(const Ecs* ecs, EcsEntityID entity);

rect GetEntityRect(const Ecs* ecs, EcsEntityID entity);

#endif /* !TRANSFORM_H */