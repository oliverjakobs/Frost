#ifndef ANIMATION_CONDITIONS_H
#define ANIMATION_CONDITIONS_H

#include "Ecs/Ecs.h"

int AnimationConditionJump(Ecs* ecs, EcsEntityID e, int s);
int AnimationConditionFall(Ecs* ecs, EcsEntityID e, int s);
int AnimationConditionWalk(Ecs* ecs, EcsEntityID e, int s);
int AnimationConditionIdle(Ecs* ecs, EcsEntityID e, int s);

#endif /* !ANIMATION_CONDITIONS_H */
