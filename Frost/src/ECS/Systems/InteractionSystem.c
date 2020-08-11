#include "InteractionSystem.h"

void InteractionSystem(Ecs* ecs, ComponentTable* components, float deltatime)
{
	COMPONENT_MAP_ITERATE_FOR(clib_array_get(&components->table, COMPONENT_INTERACTOR), interactor_iter)
	{
		Interactor* interactor = ComponentMapIterValue(interactor_iter);
		vec2 interactor_center = EntityGetCenter(ComponentMapIterKey(interactor_iter), components);

		COMPONENT_MAP_ITERATE_FOR(clib_array_get(&components->table, COMPONENT_INTERACTION), iter)
		{
			Interaction* interaction = ComponentMapIterValue(iter);

			if (interaction->type <= interactor->type)
			{
				EntityID interaction_entity = ComponentMapIterKey(iter);
				vec2 center = EntityGetCenter(interaction_entity, components);

				if (vec2_distance(center, interactor_center) <= interaction->radius)
					printf("Interaction with %d\n", interaction_entity);
			}
		}
	}
}
