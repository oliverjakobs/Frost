#include "AnimationLoader.h"
#include <string.h>

void RegisterAnimationCondition()
{
	AnimationSystemAddCondition(AnimationConditionIdle);
	AnimationSystemAddCondition(AnimationConditionFall);
	AnimationSystemAddCondition(AnimationConditionWalk);
	AnimationSystemAddCondition(AnimationConditionJump);
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
	return 0;
}

