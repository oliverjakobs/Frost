#ifndef ANIMATION_H
#define ANIMATION_H

#include "clib/hashmap.h"

typedef struct
{
	int start;
	int length;

	float delay;
	float clock;

	int frame;

	clib_hashmap transitions;
} Animation;

void AnimationLoad(Animation* animation, int start, int length, float delay, size_t initial);
void AnimationDestroy(Animation* animation);

void AnimationStart(Animation* animation);
void AnimationTick(Animation* animation, float deltatime);

/* Animation Conditions */
#include "ECS/Ecs.h"

typedef struct
{
	int (*func)(Ecs*, EntityID, int);
} AnimationCondition;

void AnimationConditionsInit();
void AnimationConditionsDestroy();

int AnimationConditionsRegisterCondition(const char* name, int (*condition)(Ecs*, EntityID, int));
AnimationCondition* AnimationConditionsGetCondition(const char* name);

#endif /* !ANIMATION_H */