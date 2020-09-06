#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "math/math.h"

#include "Scenes/Scene.h"

typedef enum
{
	RIGID_BODY_STATIC,
	RIGID_BODY_DYNAMIC
} RigidBodyType;

typedef struct
{
	vec2 position;
	vec2 half_size;

	vec2 offset;

	vec2 velocity;

	RigidBodyType type;

	// CollisionState
	int collides_bottom;
	int collides_top;
	int collides_left;
	int collides_right;
} RigidBody;

void RigidBodyLoad(Scene* scene, EcsEntityID entity, char* json);

void RigidBodyTick(RigidBody* body, vec2 gravity, float deltatime);
void RigidBodyResolveCollision(RigidBody* body, const RigidBody* other, vec2 oldpos);

#endif /* !RIGID_BODY_H */