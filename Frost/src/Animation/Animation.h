#ifndef ANIMATION_H
#define ANIMATION_H

#include "clib/strmap.h"

typedef struct
{
	int start;
	int length;

	float delay;
	float clock;

	int frame;

	clib_strmap transitions;
} Animation;

void AnimationLoad(Animation* animation, int start, int length, float delay, size_t initial);
void AnimationDestroy(Animation* animation);

void AnimationStart(Animation* animation);
void AnimationTick(Animation* animation, float deltatime);

void AnimationAddTransition(Animation* animation, char* name, char* next);

#endif /* !ANIMATION_H */