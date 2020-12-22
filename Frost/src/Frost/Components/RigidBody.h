#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "math/math.h"

#include "Scenes/Scene.h"

typedef enum
{
	RIGID_BODY_STATIC,
	RIGID_BODY_DYNAMIC
} RigidBodyType;

typedef enum
{
	RIGID_BODY_FILTER_NONE		= 0,		/* 00000000 */
	RIGID_BODY_FILTER_WORLD		= 1 << 0,	/* 00000001 */
	RIGID_BODY_FILTER_PLAYER	= 1 << 1,	/* 00000010	*/
	RIGID_BODY_FILTER_1			= 1 << 2,	/* 00000100	*/
	RIGID_BODY_FILTER_2			= 1 << 3,	/* 00001000	*/
	RIGID_BODY_FILTER_3			= 1 << 4,	/* 00010000	*/
	RIGID_BODY_FILTER_4			= 1 << 5,	/* 00100000	*/
	RIGID_BODY_FILTER_5			= 1 << 6,	/* 01000000	*/
	RIGID_BODY_FILTER_6			= 1 << 7	/* 10000000	*/
} RigidBodyFilter;

typedef struct
{
	vec2 position;
	vec2 half_size;

	vec2 offset;

	vec2 velocity;

	RigidBodyType type;
	RigidBodyFilter filter;

	// CollisionState
	int collides_bottom;
	int collides_top;
	int collides_left;
	int collides_right;
} RigidBody;

void RigidBodyLoad(Scene* scene, EcsEntityID entity, vec2 pos, int z_index, char* json);

void RigidBodyTick(RigidBody* body, vec2 gravity, float deltatime);
void RigidBodyResolveCollision(RigidBody* body, const RigidBody* other, vec2 oldpos);

#endif /* !RIGID_BODY_H */