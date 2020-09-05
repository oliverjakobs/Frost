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

	if (clib_hashmap_alloc(&animation->transitions, clib_hash_string, clib_hashmap_str_cmp, initial) == CLIB_HASHMAP_OK)
	{
		clib_hashmap_set_key_alloc_funcs(&animation->transitions, clib_hashmap_str_alloc, clib_hashmap_str_free);
		clib_hashmap_set_value_alloc_funcs(&animation->transitions, clib_hashmap_str_alloc, clib_hashmap_str_free);
	}
}

void AnimationDestroy(Animation* animation)
{
	clib_hashmap_free(&animation->transitions);
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

static clib_hashmap condition_table; /* <str, AnimationCondition> */

void AnimationConditionsInit()
{
	if (clib_hashmap_alloc(&condition_table, clib_hash_string, clib_hashmap_str_cmp, 0) == CLIB_HASHMAP_OK)
		clib_hashmap_set_key_alloc_funcs(&condition_table, clib_hashmap_str_alloc, clib_hashmap_str_free);
}

void AnimationConditionsDestroy()
{
	CLIB_HASHMAP_ITERATE_FOR(&condition_table, iter)
	{
		free(clib_hashmap_iter_get_value(iter));
		clib_hashmap_iter_remove(&condition_table, iter);
	}
	clib_hashmap_free(&condition_table);
}

int AnimationConditionsRegisterCondition(const char* name, int(*condition)(Ecs*, EntityID, int))
{
	AnimationCondition* value = malloc(sizeof(AnimationCondition));

	if (!value) return 0;

	value->func = condition;
	if (clib_hashmap_insert(&condition_table, name, value) == value)
		return 1;

	free(value);
	return 0;
}

AnimationCondition* AnimationConditionsGetCondition(const char* name)
{
	return clib_hashmap_find(&condition_table, name);
}
