#include "InteractionSystem.h"

void InteractionSystem(Ecs* ecs, ComponentTable* components, float deltatime)
{
	CLIB_DICT_ITERATE_FOR(&components->table[COMPONENT_INTERACTOR], interactor_iter)
	{
		Interactor* interactor = clib_dict_iter_get_value(interactor_iter);
		vec2 interactor_center = EntityGetCenter(clib_dict_iter_get_key(interactor_iter), components);

		CLIB_DICT_ITERATE_FOR(&components->table[COMPONENT_INTERACTION], iter)
		{
			Interaction* interaction = clib_dict_iter_get_value(iter);

			if (interaction->type <= interactor->type)
			{
				char* interaction_entity = clib_dict_iter_get_key(iter);
				vec2 center = EntityGetCenter(interaction_entity, components);

				if (vec2_distance(center, interactor_center) <= interaction->radius)
					printf("Interaction with %s\n", interaction_entity);
			}
		}
	}
}
