#include "Interaction.h"

#include "Frost/Frost.h"

#include "toolbox/tb_json.h"
#include "Application/Debugger.h"

void InteractableLoad(Scene* scene, EcsEntityID entity, char* json)
{
	if (!EcsGetDataComponent(&scene->ecs, entity, COMPONENT_TRANSFORM))
	{
		DEBUG_ERROR("[ECS] Interactable requires Transform\n");
		return;
	}

	tb_json_element element;
	tb_json_read(json, &element, "{'interaction'");
	if (element.error == TB_JSON_OK)
	{
		Interactable comp;
		comp.type = tb_json_parse(element.value, "{'type'", NULL, (tb_json_parse_func)InteractionParseType);
		comp.interaction = tb_json_parse(element.value, "{'interaction'", NULL, (tb_json_parse_func)InteractionParse);
		comp.key = tb_json_parse(element.value, "{'key'", NULL, (tb_json_parse_func)InputParseKeyCode);

		comp.range_min = tb_json_float(element.value, "{'range'[0", NULL, 0.0f);
		comp.range_max = tb_json_float(element.value, "{'range'[1", NULL, 0.0f);

		if (comp.type == INTERACTION_TYPE_TIMED)
			comp.time = tb_json_float(element.value, "{'time'", NULL, 0.0f);
		else
			comp.time = 0.0f;

		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_INTERACTABLE, &comp);
	}
}

int InteractionToggleDoor(Ecs* ecs, EcsEntityID entity)
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

int DispatchInteraction(Ecs* ecs, EcsEntityID entity, Interaction interaction, int active)
{
	switch (interaction)
	{
	case INTERACTION_TOGGLE_DOOR:		return InteractionToggleDoor(ecs, entity);
	case INTERACTION_OPEN_INVENTORY:	return InteractionOpenInventory(ecs, entity, active);
	default: return 0;
	}
}

Interaction InteractionParse(const char* str, size_t max_count)
{
	if (strncmp(str, "TOGGLE_DOOR", max_count) == 0) return INTERACTION_TOGGLE_DOOR;
	if (strncmp(str, "OPEN_INVENTORY", max_count) == 0) return INTERACTION_OPEN_INVENTORY;

	return INTERACTION_NONE;
}

InteractionType InteractionParseType(const char* str, size_t max_count)
{
	if (strncmp(str, "TOGGLE", max_count) == 0) return INTERACTION_TYPE_TOGGLE;
	if (strncmp(str, "RANGED", max_count) == 0) return INTERACTION_TYPE_RANGED;
	if (strncmp(str, "TIMED", max_count) == 0) return INTERACTION_TYPE_TIMED;

	return INTERACTION_TYPE_TOGGLE;
}

