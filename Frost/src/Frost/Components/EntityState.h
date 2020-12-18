#ifndef ENTITY_STATE_H
#define ENTITY_STATE_H

#include "ECS/Ecs.h"

typedef enum
{
    ENTITY_STATE_NULL,
    ENTITY_STATE_IDLE,
    ENTITY_STATE_WALK,
    ENTITY_STATE_JUMP,
    ENTITY_STATE_FALL,
    NUM_ENTITY_STATES
} EntityState;

EntityState EntityGetState(Ecs* ecs, EcsEntityID entity);
const char* EntityGetStateString(Ecs* ecs, EcsEntityID entity);

const char* EntityStateToString(EntityState value);
EntityState EntityStateFromString(const char* str);

#endif /* !ENTITY_STATE_H */
