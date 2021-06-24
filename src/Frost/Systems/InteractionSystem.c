#include "InteractionSystem.h"

#include "Frost/Frost.h"

void InteractionSystem(Ecs* ecs, const Scene* scene, float deltatime)
{
	EcsMap* p_map = EcsGetComponentMap(ecs, COMPONENT_PLAYER);
	for (EcsMapIter* p_iter = EcsMapIterator(p_map); p_iter; p_iter = EcsMapIterNext(p_map, p_iter))
	{
		Player* player = EcsMapIterValue(p_iter);
		vec2 p_center = GetEntityCenter(ecs, EcsMapIterKey(p_iter));

		EcsMap* interactions = EcsGetComponentMap(ecs, COMPONENT_INTERACTABLE);
		for (EcsMapIter* iter = EcsMapIterator(interactions); iter; iter = EcsMapIterNext(interactions, iter))
		{
			Interactable* interactable = EcsMapIterValue(iter);
			EcsEntityID interactable_entity = EcsMapIterKey(iter);
			vec2 center = GetEntityCenter(ecs, interactable_entity);

			float distance = vec2_distance(center, p_center);
			DispatchInteraction(ecs, interactable_entity, interactable, distance);
		}
	}
}
