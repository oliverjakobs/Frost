#include "InteractionSystem.h"

#include "Frost/FrostEcs.h"

void InteractionSystem(Ecs* ecs, float deltatime)
{
	COMPONENT_MAP_ITERATE(EcsGetComponentMap(ecs, COMPONENT_INTERACTOR), interactor_iter)
	{
		Interactor* interactor = ComponentMapIterValue(interactor_iter);
		vec2 interactor_center = GetEntityCenter(ecs, ComponentMapIterKey(interactor_iter));

		COMPONENT_MAP_ITERATE(EcsGetComponentMap(ecs, COMPONENT_INTERACTION), iter)
		{
			Interaction* interaction = ComponentMapIterValue(iter);

			if (interaction->type <= interactor->type)
			{
				EntityID interaction_entity = ComponentMapIterKey(iter);
				vec2 center = GetEntityCenter(ecs, interaction_entity);

				if (vec2_distance(center, interactor_center) <= interaction->radius)
					printf("Interaction with %d\n", interaction_entity);
			}
		}
	}
}
