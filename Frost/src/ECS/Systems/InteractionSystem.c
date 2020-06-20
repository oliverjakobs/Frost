#include "InteractionSystem.h"

void EcsSystemInteraction(ComponentTable* components, const char* entity, float deltatime)
{
	EcsInteractorComponent* interactor = ComponentTableGetComponent(components, entity, COMPONENT_INTERACTOR);

	if (!interactor) return;

	vec2 interactor_center = EntityGetCenter(entity, components);

	CLIB_DICT_ITERATE_FOR(&components->components[COMPONENT_INTERACTION], iter)
	{
		EcsInteractionComponent* interaction = clib_dict_iter_get_value(iter);

		if (interaction->type <= interactor->type)
		{
			char* interaction_entity = clib_dict_iter_get_key(iter);
			vec2 center = EntityGetCenter(interaction_entity, components);

			if (vec2_distance(center, interactor_center) <= interaction->radius)
				printf("Interaction with %s\n", interaction_entity);
		}
	}
}
