#include "Sprite.h"

#include "Frost/FrostParser.h"

void SpriteLoad(char* json, Ecs* ecs, EcsEntityID entity, const Resources* res, int z_index, int variant)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);
	if (!transform)
	{
		MINIMAL_ERROR("[ECS] Sprite requires Transform");
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

		sprite.frame = FrostMatchVariant(element.value, "{'frame'", variant);

		char texture[APPLICATION_STR_LEN];
		tb_json_string(element.value, "{'texture'", texture, APPLICATION_STR_LEN, NULL);

		sprite.texture = ResourcesGetTexture2D(res, texture);

		if (sprite.texture)
		{
			EcsAddDataComponent(ecs, entity, COMPONENT_SPRITE, &sprite);
			EcsAddOrderComponent(ecs, entity, COMPONENT_Z_INDEX, &z_index);
		}
		else
		{
			MINIMAL_ERROR("[Scenes] Found sprite but couldn't find texture");
		}
	}
}

void SpriteGetSrcRect(const Sprite* sprite, float* src_x, float* src_y, float* src_w, float* src_h)
{
	GetTexture2DSrcRect(sprite->texture, sprite->frame, src_x, src_y, src_w, src_h);

	if (sprite->flip == SPRITE_FLIP_HORIZONTAL || sprite->flip == SPRITE_FLIP_BOTH)
	{
		*src_x += *src_w;
		*src_w = -*src_w;
	}

	if (sprite->flip == SPRITE_FLIP_VERTICAL || sprite->flip == SPRITE_FLIP_BOTH)
	{
		*src_y += *src_h;
		*src_h = -*src_h;
	}
}
