#ifndef ANIMATION_CONDITIONS_H
#define ANIMATION_CONDITIONS_H

#include "Entity/Components.hpp"


bool AnimationConditionJump(Entity* e, int s);
bool AnimationConditionFall(Entity* e, int s);
bool AnimationConditionWalk(Entity* e, int s);
bool AnimationConditionIdle(Entity* e, int s);

#endif /* !ANIMATION_CONDITIONS_H */

