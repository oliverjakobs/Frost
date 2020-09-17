#ifndef ANIMATION_LOADER_H
#define ANIMATION_LOADER_H

#include "Frost/AnimationConditions.h"

typedef enum
{
	CONDITION_JUMP,
	CONDITION_FALL,
	CONDITION_WALK,
	CONDITION_IDLE
} AnimationCondition;

void RegisterAnimationCondition();

#endif /* !ANIMATION_LOADER_H */
