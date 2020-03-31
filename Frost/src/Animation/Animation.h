#ifndef ANIMATION_H
#define ANIMATION_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "clib/hashmap.h"

#define ANIMATION_STRLEN			32

typedef struct
{
	char name[ANIMATION_STRLEN];
	char next[ANIMATION_STRLEN];
} AnimationTransition;

CLIB_HASHMAP_DECLARE_FUNCS(transition, char, AnimationTransition)

typedef struct
{
	int start;
	int length;

	float delay;
	float clock;

	int frame;

	clib_hashmap transitions;	/* <str,str> */
} Animation;

void AnimationLoad(Animation* animation, int start, int length, float delay, size_t initial);
void AnimationDestroy(Animation* animation);

void AnimationStart(Animation* animation);
void AnimationTick(Animation* animation, float deltatime);

void AnimationAddTransition(Animation* animation, const char* name, const char* next);

#ifdef __cplusplus
}
#endif

#endif /* !ANIMATION_H */