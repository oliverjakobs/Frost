#include "Interaction.h"

#include "Frost/FrostParser.h"
#include "toolbox/tb_algorithm.h"

void InteractableLoad(char* ini, Ecs* ecs, EcsEntityID entity)
{
	if (!EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM))
	{
		MINIMAL_ERROR("[ECS] Interactable requires Transform");
		return;
	}

	tb_ini_element element;
	tb_ini_query(ini, "interaction", NULL, &element);
	if (element.error == TB_INI_OK)
	{
		Interactable comp;
		comp.type = tb_ini_parse(element.start, NULL, "type", FrostParseInteractionType);
		comp.interaction = tb_ini_parse(element.start, NULL, "interaction", FrostParseInteraction);
		comp.key = tb_ini_parse(element.start, NULL, "key", FrostParseInputKeyCode);

		comp.range_min = tb_ini_float(element.start, NULL, "range_min", 0.0f);
		comp.range_max = tb_ini_float(element.start, NULL, "range_max", 0.0f);

		comp.time = (comp.type == INTERACTION_TYPE_TIMED) ? tb_ini_float(element.start, NULL, "time", 0.0f) : 0.0f;

		EcsAddDataComponent(ecs, entity, COMPONENT_INTERACTABLE, &comp);
	}
}

int InteractionToggleDoor(Ecs* ecs, EcsEntityID entity, int i)
{
	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);
	if (!body) return 0;

	Sprite* sprite = EcsGetDataComponent(ecs, entity, COMPONENT_SPRITE);

	body->filter ^= RIGID_BODY_FILTER_DOOR;
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

void DispatchInteraction(Ecs* ecs, EcsEntityID entity, const Interactable* interactable, float distance)
{
	if (interactable->type == INTERACTION_TYPE_TOGGLE)
	{
		if (tb_betweenf(interactable->range_min, interactable->range_max, distance) && MinimalKeyReleased(interactable->key))
			EcsEventThrow(ecs, interactable->interaction, entity, 1);
	}
	else if (interactable->type == INTERACTION_TYPE_RANGED)
	{
		if (distance <= interactable->range_min && MinimalKeyReleased(interactable->key))
			EcsEventThrow(ecs, interactable->interaction, entity, 1);
		else if (distance >= interactable->range_max)
			EcsEventThrow(ecs, interactable->interaction, entity, 0);
	}
}