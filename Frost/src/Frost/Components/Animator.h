#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Scenes/Scene.h"

/* ---------------------------------| Animation |----------------------------------- */
typedef struct
{
	int start;
	int length;

	float delay;
	float clock;

	int frame;

	tb_hashmap transitions;
} Animation;

void AnimationLoad(Animation* animation, int start, int length, float delay, size_t initial);
void AnimationDestroy(Animation* animation);

void AnimationStart(Animation* animation);
void AnimationTick(Animation* animation, float deltatime);

/* ---------------------------------| AnimationCondition |-------------------------- */
typedef struct
{
	int (*func)(Ecs*, EcsEntityID, int);
} AnimationCondition;

void AnimationConditionsInit();
void AnimationConditionsDestroy();

int AnimationConditionsRegisterCondition(const char* name, int (*condition)(Ecs*, EcsEntityID, int));
AnimationCondition* AnimationConditionsGetCondition(const char* name);

/* ---------------------------------| Animator |------------------------------------ */
typedef struct
{
	tb_hashmap animations;	/* <str, Animation*> */
	const char* current;
} Animator;

void AnimatorFree(void* block);

void AnimatorLoad(Scene* scene, EcsEntityID entity, char* json);

#endif /* !ANIMATOR_H */
