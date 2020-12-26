#include "Player.h"

#include "Frost/Frost.h"

#include "toolbox/tb_json.h"

void PlayerLoad(Scene* scene, EcsEntityID entity, char* json)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'player'");
	if (element.error == TB_JSON_OK)
	{
		Player comp;
		comp.move_left = KEY_A;
		comp.move_right = KEY_D;
		comp.jump = KEY_SPACE;

		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_PLAYER, &comp);
	}
}
