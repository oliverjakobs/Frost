#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Animation.h"

struct ecs_entity;

typedef struct 
{
	clib_dict conditions;	/* <str, (int (*condition)(EcsEntity*, int))> */
	clib_dict animations;	/* <str, Animation*> */
	char* current;
} Animator;

void AnimatorInit(Animator* animator);
void AnimatorDestroy(Animator* animator);

int AnimatorRegisterConition(Animator* animator, const char* name, int (*condition)(struct ecs_entity*, int));
int AnimatorAddAnimation(Animator* animator, const char* name, Animation* animation);

void AnimatorTick(Animator* animator, struct ecs_entity* entity, float deltatime);
void AnimatorPlay(Animator* animator, const char* name);

int AnimatorGetFrame(Animator* animator);

void AnimatorDebugPrint(Animator* animator);

#endif /* !ANIMATOR_H */