#include "Animation.h"

#include <string.h>
#include <stdio.h>

void AnimationLoad(Animation* animation, int start, int length, float delay, size_t initial)
{
	animation->start = start;
	animation->length = length;
	animation->delay = delay;
	animation->clock = 0.0f;
	animation->frame = 0;

	clib_strmap_alloc(&animation->transitions, initial);
}

void AnimationDestroy(Animation* animation)
{
	clib_strmap_free(&animation->transitions);
}

void AnimationStart(Animation* animation)
{
	animation->frame = animation->start;
	animation->clock = 0.0f;
}

void AnimationTick(Animation* animation, float deltatime)
{
	animation->clock += deltatime;

	// change frame
	if (animation->clock > animation->delay)
	{
		animation->clock = 0.0f;
		animation->frame++;
	}

	// restart animation
	if (animation->frame >= animation->start + animation->length || animation->frame < animation->start)
	{
		animation->frame = animation->start;
		animation->clock = 0.0f;
	}
}

void AnimationAddTransition(Animation* animation, char* name, char* next)
{
	clib_strmap_insert(&animation->transitions, name, next);
}


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
