#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Animation/Animation.h"

#include "Scenes/Scene.h"

typedef struct
{
	clib_hashmap animations;	/* <str, Animation*> */
	char* current;
} Animator;

void AnimatorFree(void* block);

void AnimatorLoad(Scene* scene, EntityID entity, char* json);

#endif /* !ANIMATOR_H */
