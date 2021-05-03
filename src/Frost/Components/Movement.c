#include "Movement.h"

#include "Frost/Frost.h"

#include "toolbox/tb_json.h"

void MovementLoad(char* json, Ecs* ecs, EcsEntityID entity)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'movement'");

	if (element.error == TB_JSON_OK)
	{
		Movement comp;

		comp.direction = MOVEMENT_LEFT;
		comp.speed = tb_json_float(element.value, "{'speed'", NULL, 0.0f);
		comp.jump_power = tb_json_float(element.value, "{'jumppower'", NULL, 0.0f);

		EcsAddDataComponent(ecs, entity, COMPONENT_MOVEMENT, &comp);
	}
}
