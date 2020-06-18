#ifndef ECS_PHYSICS_SYSTEM_H
#define ECS_PHYSICS_SYSTEM_H

#include "../Components.h"

void EcsSystemPhysics(ComponentTable* components, const char* entity, float deltatime);

#endif /* !ECS_PHYSICS_SYSTEM_H */