#include "AnimationLoader.h"

void RegisterAnimationCondition()
{
	AnimationConditionsRegisterCondition(AnimationConditionJump);
	AnimationConditionsRegisterCondition(AnimationConditionFall);
	AnimationConditionsRegisterCondition(AnimationConditionWalk);
	AnimationConditionsRegisterCondition(AnimationConditionIdle);
}

