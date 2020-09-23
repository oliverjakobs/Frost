#ifndef ANIMATION_LOADER_H
#define ANIMATION_LOADER_H

#include "Frost/AnimationConditions.h"

typedef enum
{
	ANIMATION_IDLE,
	ANIMATION_FALL,
	ANIMATION_WALK,
	ANIMATION_JUMP
} Animations;

const char* AnimationsToString(Animations value);
Animations AnimationsFromString(const char* str);

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

#endif /* !ANIMATION_LOADER_H */
