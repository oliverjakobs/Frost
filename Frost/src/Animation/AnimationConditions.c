#include "AnimationConditions.h"

#include <stdlib.h>

#include "ECS/Components.h"

static clib_hashmap condition_table; /* <str, AnimationCondition> */

void AnimationConditionsInit()
{
	clib_dict_alloc(&condition_table, 0);
}

void AnimationConditionsDestroy()
{
	CLIB_DICT_ITERATE_FOR(&condition_table, iter)
	{
		free(clib_dict_iter_get_value(iter));
		clib_dict_iter_remove(&condition_table, iter);
	}
}

int AnimationConditionsRegisterCondition(const char* name, int(*condition)(Ecs*, EntityID, int))
{
	AnimationCondition* value = malloc(sizeof(AnimationCondition));

	if (!value) return 0;

	value->func = condition;
	if (clib_dict_insert(&condition_table, name, value) == value)
		return 1;

	free(value);
	return 0;
}

AnimationCondition* AnimationConditionsGetCondition(const char* name)
{
	return clib_dict_find(&condition_table, name);
}

int AnimationConditionJump(Ecs* ecs, EntityID entity, int s)
{
	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);

	if (body) return body->velocity.y > 0.0f;

	return 0;
}

int AnimationConditionFall(Ecs* ecs, EntityID entity, int s)
{
	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);

	if (body) return !body->collides_bottom && body->velocity.y <= 0.0f;

	return 0;
}

int AnimationConditionWalk(Ecs* ecs, EntityID entity, int s)
{
	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);

	if (body) return body->collides_bottom && body->velocity.x != 0.0f;

	return 0;
}

int AnimationConditionIdle(Ecs* ecs, EntityID entity, int s)
{
	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);

	if (body) return body->collides_bottom && body->velocity.x == 0.0f;

	return 0;
}
