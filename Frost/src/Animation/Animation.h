#ifndef ANIMATION_H
#define ANIMATION_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "clib/strmap.h"

#define ANIMATION_STRLEN			32


typedef struct
{
	int start;
	int length;

	float delay;
	float clock;

	int frame;

	clib_strmap transitions;	/* <str,str> */
} Animation;

void AnimationLoad(Animation* animation, int start, int length, float delay, size_t initial);
void AnimationDestroy(Animation* animation);

void AnimationStart(Animation* animation);
void AnimationTick(Animation* animation, float deltatime);

void AnimationAddTransition(Animation* animation, char* name, char* next);

#ifdef __cplusplus
}
#endif

#endif /* !ANIMATION_H */