#include "Interaction.h"

#include "Frost/Frost.h"

#include "toolbox/tb_json.h"

void InteractableLoad(Scene* scene, EcsEntityID entity, char* json)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'interaction'");
	if (element.error == TB_JSON_OK)
	{
		/* TODO: different values for different types */
		Interactable comp;
		comp.type = (InteractionType)tb_json_int(element.value, "{'type'", NULL, 0);
		comp.min_radius = tb_json_float(element.value, "{'min-radius'", NULL, 0.0f);
		comp.max_radius = tb_json_float(element.value, "{'max-radius'", NULL, 0.0f);
		comp.interaction = (Interaction)tb_json_int(element.value, "{'interaction'", NULL, 0);
		comp.key = KEY_E;

		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_INTERACTABLE, &comp);
	}
}

int InteractionToggleDoor(Ecs* ecs, EcsEntityID entity)
{
	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);
	if (!body) return 0;

	Sprite* sprite = EcsGetDataComponent(ecs, entity, COMPONENT_SPRITE);

	body->filter ^= RIGID_BODY_FILTER_PLAYER;
	sprite->frame = !sprite->frame;

	return 1;
}

int InteractionOpenInventory(Ecs* ecs, EcsEntityID entity, int active)
{
	Inventory* inv = EcsGetDataComponent(ecs, entity, COMPONENT_INVENTORY);
	if (!inv || inv->state == INVENTORY_FIXED) return 0;

	if (active) InventoryToggle(inv);
	else inv->state = INVENTORY_CLOSED;
	return 1;
}

int DispatchInteraction(Ecs* ecs, EcsEntityID entity, Interaction interaction, int active)
{
	switch (interaction)
	{
	case INTERACTION_TOGGLE_DOOR:		return InteractionToggleDoor(ecs, entity);
	case INTERACTION_OPEN_INVENTORY:	return InteractionOpenInventory(ecs, entity, active);
	default: return 0;
	}
}

