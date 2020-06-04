#ifndef ANIMATION_CONDITIONS_H
#define ANIMATION_CONDITIONS_H

#include "ECS/Entity.h"

int AnimationConditionJump(EcsEntity* e, int s);
int AnimationConditionFall(EcsEntity* e, int s);
int AnimationConditionWalk(EcsEntity* e, int s);
int AnimationConditionIdle(EcsEntity* e, int s);

#endif /* !ANIMATION_CONDITIONS_H */
