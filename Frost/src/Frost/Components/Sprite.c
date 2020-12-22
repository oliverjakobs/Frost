#include "Sprite.h"

#include "Frost/Frost.h"

#include "toolbox/tb_json.h"

#include "Application/Debugger.h"

void SpriteLoad(Scene* scene, EcsEntityID entity, vec2 pos, int z_index, char* json)
{
	Transform* transform = EcsGetDataComponent(&scene->ecs, entity, COMPONENT_TRANSFORM);
	if (!transform)
	{
		DEBUG_ERROR("[Scenes] Sprite requires Transform\n");
		return;
	}

	tb_json_element element;
	tb_json_read(json, &element, "{'sprite'");
	if (element.error == TB_JSON_OK)
	{
		Sprite sprite;
		sprite.flip = SPRITE_FLIP_NONE;
		sprite.width = tb_json_float(element.value, "{'size'[0", NULL, transform->size.x);
		sprite.height = tb_json_float(element.value, "{'size'[1", NULL, transform->size.y);

		sprite.frame = tb_json_int(element.value, "{'frame'", NULL, 0);

		char texture[APPLICATION_STR_LEN];
		tb_json_string(element.value, "{'texture'", texture, APPLICATION_STR_LEN, NULL);

		sprite.texture = ResourcesGetTexture2D(scene->resources, texture);

		if (sprite.texture)
		{
			EcsAddDataComponent(&scene->ecs, entity, COMPONENT_SPRITE, &sprite);
			EcsAddOrderComponent(&scene->ecs, entity, COMPONENT_Z_INDEX, &z_index);
		}
		else
		{
			DEBUG_ERROR("[Scenes] Found sprite but couldn't find texture\n");
		}
	}
}
