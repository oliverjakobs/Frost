#include "AnimationComponent.h"


void AnimationComponentInit(EcsAnimationComponent* comp)
{
	clib_dict_init(&comp->conditions, 0);
	clib_dict_init(&comp->animations, 0);

	AnimationComponentRegisterCondition(comp, "condition_jump", AnimationConditionJump);
	AnimationComponentRegisterCondition(comp, "condition_fall", AnimationConditionFall);
	AnimationComponentRegisterCondition(comp, "condition_walk", AnimationConditionWalk);
	AnimationComponentRegisterCondition(comp, "condition_idle", AnimationConditionIdle);

	comp->current = NULL;
}

void AnimationComponentDestroy(EcsAnimationComponent* comp)
{
	CLIB_DICT_ITERATE_FOR(&comp->animations, iter)
	{
		free(clib_dict_iter_get_value(iter));
		clib_dict_iter_remove(&comp->animations, iter);
	}

	CLIB_DICT_ITERATE_FOR(&comp->conditions, iter)
	{
		free(clib_dict_iter_get_value(iter));
		clib_dict_iter_remove(&comp->conditions, iter);
	}
}

int AnimationComponentRegisterCondition(EcsAnimationComponent* comp, const char* name, int(*condition)(ComponentTable*, const char*, int))
{
	AnimationCondition* value = (AnimationCondition*)malloc(sizeof(AnimationCondition));

	if (value)
	{
		value->func = condition;
		if (clib_dict_insert(&comp->conditions, name, value) == value)
			return 1;
	}

	free(value);
	return 0;
}

int AnimationComponentAddAnimation(EcsAnimationComponent* comp, const char* name, Animation* animation)
{
	if (clib_dict_insert(&comp->animations, name, animation) != animation)
	{
		AnimationDestroy(animation);
		free(animation);
		return 0;
	}

	if (comp->current == NULL)
		comp->current = clib_dict_get_key_ptr(&comp->animations, name);

	return 1;
}
