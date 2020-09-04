#include "Sprite.h"

#include "Frost/FrostEcs.h"

#include "json/tb_json.h"

#include "Application/Debugger.h"

void SpriteLoad(Scene* scene, EntityID entity, char* json)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'sprite'");
	if (element.error == TB_JSON_OK)
	{
		Sprite sprite;
		sprite.flip = SPRITE_FLIP_NONE;
		sprite.width = tb_json_float((char*)element.value, "{'size'[0", NULL, -1.0f);
		sprite.height = tb_json_float((char*)element.value, "{'size'[1", NULL, -1.0f);

		Transform* transform = EcsGetDataComponent(&scene->ecs, entity, COMPONENT_TRANSFORM);

		if (sprite.width < 0.0f && transform) sprite.width = transform->size.x;
		if (sprite.height < 0.0f && transform) sprite.height = transform->size.y;

		sprite.frame = tb_json_int((char*)element.value, "{'frame'", NULL, 0);

		char texture[APPLICATION_STR_LEN];
		tb_json_string((char*)element.value, "{'texture'", texture, APPLICATION_STR_LEN, NULL);

		sprite.texture = ResourcesGetTexture2D(scene->resources, texture);

		if (sprite.texture)
			EcsAddDataComponent(&scene->ecs, entity, COMPONENT_SPRITE, &sprite);
		else
			DEBUG_ERROR("[Scenes] Found sprite but couldn't find texture");
	}
}
