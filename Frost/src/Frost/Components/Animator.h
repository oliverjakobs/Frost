#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Scenes/Scene.h"
#include "EntityState.h"

typedef struct
{
	int start;
	int length;

	float delay;
} Animation;

typedef struct
{
	Animation animations[NUM_ENTITY_STATES];
	EntityState current;
	int frame;

	float clock;
} Animator;

void AnimatorFree(void* block);

void AnimatorStart(Animator* animator, int start);

void AnimatorLoad(Scene* scene, EcsEntityID entity, vec2 pos, int z_index, char* json);

#endif /* !ANIMATOR_H */
