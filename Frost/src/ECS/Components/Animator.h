#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Animation/Animation.h"

typedef struct
{
	clib_hashmap animations;	/* <str, Animation*> */
	char* current;
} Animator;

void AnimatorInit(Animator* animator);
void AnimatorDestroy(Animator* animator);

int AnimatorAddAnimation(Animator* animator, const char* name, Animation* animation);

#endif /* !ANIMATOR_H */
