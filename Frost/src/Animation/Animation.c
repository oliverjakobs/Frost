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

	clib_strmap_init(&animation->transitions, initial);
}

void AnimationDestroy(Animation* animation)
{
	clib_strmap_destroy(&animation->transitions);
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
