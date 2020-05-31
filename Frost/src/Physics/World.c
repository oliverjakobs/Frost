#include "World.h"

CLIB_DYNAMIC_ARRAY_DEFINE_FUNCS(body, Body)

void WorldLoad(World* world, const vec2 gravity)
{
	world->gravity = gravity;
	clib_dynamic_array_init(&world->bodies, WORLD_INITIAL_SIZE);
}

void WorldDestroy(World* world)
{
	clib_dynamic_array_free(&world->bodies);
}

void WorldAddBody(World* world, Body* body)
{
	body->world = world;
	body_dynamic_array_push(&world->bodies, body);
}

void WorldRemoveBody(World* world, Body* body)
{
	for (size_t i = 0; i < world->bodies.size; i++)
	{
		Body* b = body_dynamic_array_get(&world->bodies, i);

		if (b == body)
		{
			b->world = NULL;
			body_dynamic_array_delete(&world->bodies, i);
			return;
		}
	}
}

void WorldTick(World* world, float deltatime)
{
	for (size_t i = 0; i < world->bodies.size; i++)
	{
		Body* body = body_dynamic_array_get(&world->bodies, i);

		if (body->type == BODY_TYPE_STATIC)
			continue;

		vec2 oldPosition = body->position;

		BodyTick(body, deltatime);

		for (size_t i = 0; i < world->bodies.size; i++)
		{
			Body* other = body_dynamic_array_get(&world->bodies, i);

			if (body != other)
				BodyResolveCollision(body, other, oldPosition);
		}
	}
}

Body* WorldGetBody(World* world, size_t index)
{
	return body_dynamic_array_get(&world->bodies, index);
}
