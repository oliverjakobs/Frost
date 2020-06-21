#ifndef ANIMATION_CONDITIONS_H
#define ANIMATION_CONDITIONS_H

#include "AnimationManager.h"

int AnimationConditionJump(ComponentTable* components, const char* e, int s);
int AnimationConditionFall(ComponentTable* components, const char* e, int s);
int AnimationConditionWalk(ComponentTable* components, const char* e, int s);
int AnimationConditionIdle(ComponentTable* components, const char* e, int s);

#endif /* !ANIMATION_CONDITIONS_H */
