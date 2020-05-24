#include "Animation.h"

#include <string.h>

CLIB_HASHMAP_DEFINE_FUNCS(transition, char, AnimationTransition)

void AnimationLoad(Animation* animation, int start, int length, float delay, size_t initial)
{
	animation->start = start;
	animation->length = length;
	animation->delay = delay;
	animation->clock = 0.0f;
	animation->frame = 0;

	clib_hashmap_init(&animation->transitions, clib_hashmap_hash_string, clib_hashmap_compare_string, initial);
}

void AnimationDestroy(Animation* animation)
{
	clib_hashmap_destroy(&animation->transitions);
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

void AnimationAddTransition(Animation* animation, const char* name, const char* next)
{
	if (strlen(name) > ANIMATION_STRLEN)
		return;

	if (strlen(next) > ANIMATION_STRLEN)
		return;

	AnimationTransition* kvp = (AnimationTransition*)malloc(sizeof(AnimationTransition));

	if (kvp)
	{
		strcpy(kvp->name, name);
		strcpy(kvp->next, next);

		if (kvp && transition_hashmap_insert(&animation->transitions, kvp->name, kvp) != kvp)
			free(kvp);
	}
}
