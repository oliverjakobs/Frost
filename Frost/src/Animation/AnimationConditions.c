#include "AnimationConditions.h"

#include <stdlib.h>

typedef struct
{
	clib_hashmap table;	/* <str, (int (*condition)(const char*, int))> */
} AnimationConditionManager;

static AnimationConditionManager conditions;

void AnimationConditionsInit()
{
	clib_dict_alloc(&conditions.table, 0);
}

void AnimationConditionsDestroy()
{
	CLIB_DICT_ITERATE_FOR(&conditions.table, iter)
	{
		free(clib_dict_iter_get_value(iter));
		clib_dict_iter_remove(&conditions.table, iter);
	}
}

int AnimationConditionsRegisterCondition(const char* name, int(*condition)(ComponentTable*, EntityID, int))
{
	AnimationCondition* value = (AnimationCondition*)malloc(sizeof(AnimationCondition));

	if (value)
	{
		value->func = condition;
		if (clib_dict_insert(&conditions.table, name, value) == value)
			return 1;
	}

	free(value);
	return 0;
}

AnimationCondition* AnimationConditionsGetCondition(const char* name)
{
	return clib_dict_find(&conditions.table, name);
}

int AnimationConditionJump(ComponentTable* components, EntityID entity, int s)
{
	RigidBody* body = ComponentTableGetComponent(components, entity, COMPONENT_RIGID_BODY);

	if (body)
		return body->velocity.y > 0.0f;

	return 0;
}

int AnimationConditionFall(ComponentTable* components, EntityID entity, int s)
{
	RigidBody* body = ComponentTableGetComponent(components, entity, COMPONENT_RIGID_BODY);

	if (body)
		return !body->collides_bottom && body->velocity.y <= 0.0f;

	return 0;
}

int AnimationConditionWalk(ComponentTable* components, EntityID entity, int s)
{
	RigidBody* body = ComponentTableGetComponent(components, entity, COMPONENT_RIGID_BODY);

	if (body)
		return body->collides_bottom && body->velocity.x != 0.0f;

	return 0;
}

int AnimationConditionIdle(ComponentTable* components, EntityID entity, int s)
{
	RigidBody* body = ComponentTableGetComponent(components, entity, COMPONENT_RIGID_BODY);

	if (body)
		return body->collides_bottom && body->velocity.x == 0.0f;

	return 0;
}
