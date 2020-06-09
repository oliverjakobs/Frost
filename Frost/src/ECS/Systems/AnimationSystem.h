#ifndef ECS_ANIMATION_SYSTEM_H
#define ECS_ANIMATION_SYSTEM_H

#include "../Entity.h"

void EcsSystemAnimation(EcsEntity* entity, ComponentTable* components, float deltatime);

#endif /* !ECS_ANIMATION_SYSTEM_H */