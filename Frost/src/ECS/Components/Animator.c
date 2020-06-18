#include "Animator.h"

#include <stdlib.h>

void AnimatorInit(Animator* animator)
{
	clib_dict_alloc(&animator->conditions, 0);
	clib_dict_alloc(&animator->animations, 0);

	AnimatorRegisterCondition(animator, "condition_jump", AnimationConditionJump);
	AnimatorRegisterCondition(animator, "condition_fall", AnimationConditionFall);
	AnimatorRegisterCondition(animator, "condition_walk", AnimationConditionWalk);
	AnimatorRegisterCondition(animator, "condition_idle", AnimationConditionIdle);

	animator->current = NULL;
}

void AnimatorDestroy(Animator* animator)
{
	CLIB_DICT_ITERATE_FOR(&animator->animations, iter)
	{
		free(clib_dict_iter_get_value(iter));
		clib_dict_iter_remove(&animator->animations, iter);
	}

	CLIB_DICT_ITERATE_FOR(&animator->conditions, iter)
	{
		free(clib_dict_iter_get_value(iter));
		clib_dict_iter_remove(&animator->conditions, iter);
	}
}

int AnimatorRegisterCondition(Animator* animator, const char* name, int(*condition)(ComponentTable*, const char*, int))
{
	AnimationCondition* value = (AnimationCondition*)malloc(sizeof(AnimationCondition));

	if (value)
	{
		value->func = condition;
		if (clib_dict_insert(&animator->conditions, name, value) == value)
			return 1;
	}

	free(value);
	return 0;
}

int AnimatorAddAnimation(Animator* animator, const char* name, Animation* animation)
{
	if (clib_dict_insert(&animator->animations, name, animation) != animation)
	{
		AnimationDestroy(animation);
		free(animation);
		return 0;
	}

	if (animator->current == NULL)
		animator->current = (char*)clib_dict_get_key_ptr(&animator->animations, name);

	return 1;
}
