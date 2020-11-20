#include "InteractionSystem.h"

#include "Frost/Frost.h"

void InteractionSystem(Ecs* ecs, float deltatime)
{
	EcsComponentMap* interactors = EcsGetComponentMap(ecs, COMPONENT_INTERACTOR);
	for (EcsComponentMapIter* interactor_iter = EcsComponentMapIterator(interactors); interactor_iter; interactor_iter = EcsComponentMapIterNext(interactors, interactor_iter))
	{
		Interactor* interactor = EcsComponentMapIterValue(interactor_iter);
		vec2 interactor_center = GetEntityCenter(ecs, EcsComponentMapIterKey(interactor_iter));

		EcsComponentMap* interactions = EcsGetComponentMap(ecs, COMPONENT_INTERACTION);
		for (EcsComponentMapIter* iter = EcsComponentMapIterator(interactions); iter; iter = EcsComponentMapIterNext(interactions, iter))
		{
			Interaction* interaction = EcsComponentMapIterValue(iter);

			if (interaction->type <= interactor->type)
			{
				EcsEntityID interaction_entity = EcsComponentMapIterKey(iter);
				vec2 center = GetEntityCenter(ecs, interaction_entity);

				if (vec2_distance(center, interactor_center) <= interaction->radius)
					printf("Interaction with %d\n", interaction_entity);
			}
		}
	}
}
