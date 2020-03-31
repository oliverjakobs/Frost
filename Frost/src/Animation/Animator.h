#ifndef ANIMATOR_H
#define ANIMATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Animation.h"

#include "clib/hashmap.h"

struct ecs_entity;

typedef struct 
{
	clib_hashmap conditions;	/* <str, (int (*condition)(EcsEntity*, int))> */
	clib_hashmap animations;	/* <str, Animation*> */
	char* current;
} Animator;

void AnimatorInit(Animator* animator);
void AnimatorDestroy(Animator* animator);

void AnimatorRegisterConition(Animator* animator, const char* name, int (*condition)(struct ecs_entity*, int));
void AnimatorAddAnimation(Animator* animator, const char* name, Animation* animation);

void AnimatorTick(Animator* animator, struct ecs_entity* entity, float deltatime);
void AnimatorPlay(Animator* animator, const char* name);

int AnimatorGetFrame(Animator* animator);

void AnimatorDebugPrint(Animator* animator);

#ifdef __cplusplus
}
#endif

#endif /* !ANIMATOR_H */