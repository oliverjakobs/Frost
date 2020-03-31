#ifndef ANIMATION_CONDITIONS_H
#define ANIMATION_CONDITIONS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ECS/Entity.h"

int AnimationConditionJump(EcsEntity* e, int s);
int AnimationConditionFall(EcsEntity* e, int s);
int AnimationConditionWalk(EcsEntity* e, int s);
int AnimationConditionIdle(EcsEntity* e, int s);

#ifdef __cplusplus
}
#endif

#endif /* !ANIMATION_CONDITIONS_H */
