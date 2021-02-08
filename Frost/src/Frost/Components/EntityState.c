#include "EntityState.h"

#include "../FrostEcs.h"
#include <string.h>
#include "..\FrostParser.h"

void EntityStateLoad(Ecs* ecs, EcsEntityID entity, EntityState state)
{
	EcsAddDataComponent(ecs, entity, COMPONENT_STATE, &state);
}

EntityState EntityGetState(Ecs* ecs, EcsEntityID entity)
{
	EntityState* state = EcsGetDataComponent(ecs, entity, COMPONENT_STATE);
	return state ? *state : ENTITY_STATE_NULL;
}

const char* EntityGetStateString(Ecs* ecs, EcsEntityID entity)
{
	return FrostEntityStateToString(EntityGetState(ecs, entity));
}
