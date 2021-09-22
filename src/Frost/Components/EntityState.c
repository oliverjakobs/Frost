#include "EntityState.h"

#include "Frost/FrostParser.h"

void EntityStateLoad(Ecs* ecs, EcsEntityID entity, EntityState state)
{
    EcsAddDataComponent(ecs, entity, COMPONENT_STATE, &state);
}

void EntitySetState(Ecs* ecs, EcsEntityID entity, EntityState state)
{
    EntityState* comp = EcsGetDataComponent(ecs, entity, COMPONENT_STATE);
    if (comp) *comp = state;
}

EntityState EntityGetState(const Ecs* ecs, EcsEntityID entity)
{
    EntityState* state = EcsGetDataComponent(ecs, entity, COMPONENT_STATE);
    return state ? *state : ENTITY_STATE_NULL;
}

const char* EntityGetStateString(const Ecs* ecs, EcsEntityID entity)
{
    return FrostEntityStateToString(EntityGetState(ecs, entity));
}
