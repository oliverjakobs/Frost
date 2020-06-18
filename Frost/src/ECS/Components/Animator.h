#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Animation/Animation.h"
#include "Animation/AnimationConditions.h"

typedef struct
{
	clib_hashmap conditions;	/* <str, (int (*condition)(const char*, int))> */
	clib_hashmap animations;	/* <str, Animation*> */
	char* current;
} Animator;

void AnimatorInit(Animator* animator);
void AnimatorDestroy(Animator* animator);

int AnimatorRegisterCondition(Animator* animator, const char* name, int (*condition)(ComponentTable*, const char*, int));
int AnimatorAddAnimation(Animator* animator, const char* name, Animation* animation);

#endif /* !ANIMATOR_H */
