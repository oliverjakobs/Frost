#ifndef ECS_PLAYER_SYSTEM_H
#define ECS_PLAYER_SYSTEM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../Entity.h"

void EcsSystemPlayer(EcsEntity* entity, float deltatime);

#ifdef __cplusplus
}
#endif

#endif /* !PLAYER_SYSTEM_H */
