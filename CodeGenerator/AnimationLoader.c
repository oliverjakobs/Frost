#include "AnimationLoader.h"
#include <string.h>

const char* AnimationsToString(Animations value)
{
	if (value == ANIMATION_IDLE) return "ANIMATION_IDLE";
	if (value == ANIMATION_FALL) return "ANIMATION_FALL";
	if (value == ANIMATION_WALK) return "ANIMATION_WALK";
	if (value == ANIMATION_JUMP) return "ANIMATION_JUMP";
	return NULL;
}

Animations AnimationsFromString(const char* str)
{
	if (strcmp(str, "ANIMATION_IDLE") == 0) return ANIMATION_IDLE;
	if (strcmp(str, "ANIMATION_FALL") == 0) return ANIMATION_FALL;
	if (strcmp(str, "ANIMATION_WALK") == 0) return ANIMATION_WALK;
	if (strcmp(str, "ANIMATION_JUMP") == 0) return ANIMATION_JUMP;
	return NULL;
}

void RegisterAnimationCondition()
{
	AnimationConditionsRegisterCondition(AnimationConditionIdle);
	AnimationConditionsRegisterCondition(AnimationConditionFall);
	AnimationConditionsRegisterCondition(AnimationConditionWalk);
	AnimationConditionsRegisterCondition(AnimationConditionJump);
}

const char* AnimationConditionToString(AnimationCondition value)
{
	if (value == CONDITION_IDLE) return "CONDITION_IDLE";
	if (value == CONDITION_FALL) return "CONDITION_FALL";
	if (value == CONDITION_WALK) return "CONDITION_WALK";
	if (value == CONDITION_JUMP) return "CONDITION_JUMP";
	return NULL;
}

AnimationCondition AnimationConditionFromString(const char* str)
{
	if (strcmp(str, "CONDITION_IDLE") == 0) return CONDITION_IDLE;
	if (strcmp(str, "CONDITION_FALL") == 0) return CONDITION_FALL;
	if (strcmp(str, "CONDITION_WALK") == 0) return CONDITION_WALK;
	if (strcmp(str, "CONDITION_JUMP") == 0) return CONDITION_JUMP;
	return NULL;
}

