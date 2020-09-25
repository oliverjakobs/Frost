#ifndef ANIMATION_LOADER_H
#define ANIMATION_LOADER_H

#include "Systems/AnimationSystem.h"

typedef enum
{
	CONDITION_IDLE,
	CONDITION_FALL,
	CONDITION_WALK,
	CONDITION_JUMP
} AnimationCondition;

void RegisterAnimationCondition();

const char* AnimationConditionToString(AnimationCondition value);
AnimationCondition AnimationConditionFromString(const char* str);

#endif /* !define */
