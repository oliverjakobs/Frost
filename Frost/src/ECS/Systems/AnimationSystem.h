#ifndef ECS_ANIMATION_SYSTEM_H
#define ECS_ANIMATION_SYSTEM_H

#include "../Entity.h"

void EcsSystemAnimation(ComponentTable* components, const char* entity, float deltatime);

#endif /* !ECS_ANIMATION_SYSTEM_H */