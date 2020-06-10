#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include "Animation/Animation.h"
#include "Animation/AnimationConditions.h"

typedef struct
{
	clib_dict conditions;	/* <str, (int (*condition)(const char*, int))> */
	clib_dict animations;	/* <str, Animation*> */
	char* current;
} EcsAnimationComponent;

void AnimationComponentInit(EcsAnimationComponent* comp);
void AnimationComponentDestroy(EcsAnimationComponent* comp);

int AnimationComponentRegisterCondition(EcsAnimationComponent* comp, const char* name, int (*condition)(ComponentTable*, const char*, int));
int AnimationComponentAddAnimation(EcsAnimationComponent* comp, const char* name, Animation* animation);

#endif /* !ANIMATION_COMPONENT_H */
