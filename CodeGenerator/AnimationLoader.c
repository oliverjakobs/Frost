#include "AnimationLoader.h"
#include <string.h>

const char* AnimationsToString(Animations value)
{
	if (value == ANIMATION_IDLE) return "ANIMATION_IDLE";
	if (value == ANIMATION_IDLE) return "ANIMATION_IDLE";
	if (value == ANIMATION_IDLE) return "ANIMATION_IDLE";
	if (value == ANIMATION_IDLE) return "ANIMATION_IDLE";
	return NULL;
}

Animations AnimationsFromString(const char* str)
{
	if (strcmp(str, "ANIMATION_IDLE") == 0) return ANIMATION_IDLE;
	if (strcmp(str, "ANIMATION_IDLE") == 0) return ANIMATION_IDLE;
	if (strcmp(str, "ANIMATION_IDLE") == 0) return ANIMATION_IDLE;
	if (strcmp(str, "ANIMATION_IDLE") == 0) return ANIMATION_IDLE;
	return NULL;
}

void RegisterAnimationCondition()
{
	AnimationConditionsRegisterCondition(AnimationConditionIdle);
	AnimationConditionsRegisterCondition([CONDITION_FALL);
	AnimationConditionsRegisterCondition(AnimationConditionFall);
	AnimationConditionsRegisterCondition([CONDITION_WALK);
}

const char* AnimationConditionToString(AnimationCondition value)
{
	if (value == CONDITION_IDLE) return "CONDITION_IDLE";
	if (value == CONDITION_IDLE) return "CONDITION_IDLE";
	if (value == CONDITION_IDLE) return "CONDITION_IDLE";
	if (value == CONDITION_IDLE) return "CONDITION_IDLE";
	return NULL;
}

AnimationCondition AnimationConditionFromString(const char* str)
{
	if (strcmp(str, "CONDITION_IDLE") == 0) return CONDITION_IDLE;
	if (strcmp(str, "CONDITION_IDLE") == 0) return CONDITION_IDLE;
	if (strcmp(str, "CONDITION_IDLE") == 0) return CONDITION_IDLE;
	if (strcmp(str, "CONDITION_IDLE") == 0) return CONDITION_IDLE;
	return NULL;
}

