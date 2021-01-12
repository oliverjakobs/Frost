#include "InteractionSystem.h"

#include "Frost/FrostEcs.h"

#include "toolbox/toolbox.h"

void InteractionSystem(Ecs* ecs, float deltatime)
{
	EcsComponentMap* p_map = EcsGetComponentMap(ecs, COMPONENT_PLAYER);
	for (EcsComponentMapIter* p_iter = EcsComponentMapIterator(p_map); p_iter; p_iter = EcsComponentMapIterNext(p_map, p_iter))
	{
		Player* player = EcsComponentMapIterValue(p_iter);
		vec2 p_center = GetEntityCenter(ecs, EcsComponentMapIterKey(p_iter));

		EcsComponentMap* interactions = EcsGetComponentMap(ecs, COMPONENT_INTERACTABLE);
		for (EcsComponentMapIter* iter = EcsComponentMapIterator(interactions); iter; iter = EcsComponentMapIterNext(interactions, iter))
		{
			Interactable* interactable = EcsComponentMapIterValue(iter);
			EcsEntityID interactable_entity = EcsComponentMapIterKey(iter);
			vec2 center = GetEntityCenter(ecs, interactable_entity);

			float distance = vec2_distance(center, p_center);
			if (interactable->type == INTERACTION_TYPE_TOGGLE)
			{
				if (tb_between_f(interactable->range_min, interactable->range_max, distance) && InputKeyUp(interactable->key))
					DispatchInteraction(ecs, interactable_entity, interactable->interaction, 1);
			}
			else if (interactable->type == INTERACTION_TYPE_RANGED)
			{
				if (distance <= interactable->range_min && InputKeyUp(interactable->key))
					DispatchInteraction(ecs, interactable_entity, interactable->interaction, 1);
				else if (distance >= interactable->range_max)
					DispatchInteraction(ecs, interactable_entity, interactable->interaction, 0);
			}
		}
	}
}
