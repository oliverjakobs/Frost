#ifndef ECS_PLAYER_SYSTEM_H
#define ECS_PLAYER_SYSTEM_H

#include "../Components.h"

void EcsSystemPlayer(ComponentTable* components, const char* entity, float deltatime);

#endif /* !PLAYER_SYSTEM_H */
