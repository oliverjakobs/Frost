#include "EntityState.h"

#include "../FrostEcs.h"
#include <string.h>

EntityState EntityGetState(Ecs* ecs, EcsEntityID entity)
{
	EntityState* state = EcsGetDataComponent(ecs, entity, COMPONENT_STATE);
	return state ? *state : ENTITY_STATE_NULL;
}

const char* EntityGetStateString(Ecs* ecs, EcsEntityID entity)
{
	return EntityStateToString(EntityGetState(ecs, entity));
}

const char* EntityStateToString(EntityState value)
{
	if (value == ENTITY_STATE_NULL) return "ENTITY_STATE_NULL";
	if (value == ENTITY_STATE_IDLE) return "ENTITY_STATE_IDLE";
	if (value == ENTITY_STATE_WALK) return "ENTITY_STATE_WALK";
	if (value == ENTITY_STATE_JUMP) return "ENTITY_STATE_JUMP";
	if (value == ENTITY_STATE_FALL) return "ENTITY_STATE_FALL";
	return NULL;
}

EntityState EntityStateFromString(const char* str)
{
	if (strcmp(str, "ENTITY_STATE_IDLE") == 0) return ENTITY_STATE_IDLE;
	if (strcmp(str, "ENTITY_STATE_WALK") == 0) return ENTITY_STATE_WALK;
	if (strcmp(str, "ENTITY_STATE_JUMP") == 0) return ENTITY_STATE_JUMP;
	if (strcmp(str, "ENTITY_STATE_FALL") == 0) return ENTITY_STATE_FALL;
	return ENTITY_STATE_NULL;
}
