#ifndef ENTITY_STATE_H
#define ENTITY_STATE_H

#include "Scenes/Scene.h"

typedef enum
{
    ENTITY_STATE_NULL,
    ENTITY_STATE_IDLE,
    ENTITY_STATE_WALK,
    ENTITY_STATE_JUMP,
    ENTITY_STATE_FALL,
    NUM_ENTITY_STATES
} EntityState;

void EntityStateLoad(Ecs* ecs, EcsEntityID entity, EntityState state);

EntityState EntityGetState(Ecs* ecs, EcsEntityID entity);
const char* EntityGetStateString(Ecs* ecs, EcsEntityID entity);

#endif /* !ENTITY_STATE_H */
