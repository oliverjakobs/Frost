#ifndef ECS_ANIMATION_SYSTEM_H
#define ECS_ANIMATION_SYSTEM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../Entity.h"

void EcsSystemAnimation(EcsEntity* entity, float deltatime);

#ifdef __cplusplus
}
#endif

#endif /* !ECS_ANIMATION_SYSTEM_H */