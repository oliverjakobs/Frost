#ifndef ANIMATION_LOADER_H
#define ANIMATION_LOADER_H

#include "Frost/AnimationConditions.h"

typedef enum
{
	ANIMATION_IDLE,
	ANIMATION_IDLE,
	ANIMATION_IDLE,
	ANIMATION_IDLE
} Animations;

const char* AnimationsToString(Animations value);
Animations AnimationsFromString(const char* str);

typedef enum
{
	CONDITION_IDLE,
	CONDITION_IDLE,
	CONDITION_IDLE,
	CONDITION_IDLE
} AnimationCondition;

void RegisterAnimationCondition();

const char* AnimationConditionToString(AnimationCondition value);
AnimationCondition AnimationConditionFromString(const char* str);

#endif /* !define */
