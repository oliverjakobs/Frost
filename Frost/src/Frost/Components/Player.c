#include "Player.h"

#include "Frost/FrostParser.h"

void PlayerLoad(char* json, Ecs* ecs, EcsEntityID entity)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'player'");
	if (element.error == TB_JSON_OK)
	{
		Player comp;
		comp.move_left = KEY_A;
		comp.move_right = KEY_D;
		comp.jump = KEY_SPACE;

		EcsAddDataComponent(ecs, entity, COMPONENT_PLAYER, &comp);
	}
}
