#ifndef ANIMATION_CONDITIONS_H
#define ANIMATION_CONDITIONS_H

#include "Ecs/Ecs.h"

int AnimationConditionJump(Ecs* ecs, EntityID e, int s);
int AnimationConditionFall(Ecs* ecs, EntityID e, int s);
int AnimationConditionWalk(Ecs* ecs, EntityID e, int s);
int AnimationConditionIdle(Ecs* ecs, EntityID e, int s);

#endif /* !ANIMATION_CONDITIONS_H */
