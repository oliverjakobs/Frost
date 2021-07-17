#include "Sprite.h"

#include "Frost/FrostParser.h"

void SpriteLoad(char* ini, Ecs* ecs, EcsEntityID entity, const Resources* res, int z_index, int variant)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);
	if (!transform)
	{
		MINIMAL_ERROR("[ECS] Sprite requires Transform");
		return;
	}

	tb_ini_element element;
	tb_ini_query(ini, "sprite", NULL, &element);
	if (element.error == TB_INI_OK)
	{
		Sprite sprite;
		sprite.flip = SPRITE_FLIP_NONE;
		sprite.width = tb_ini_query_float(element.start, NULL, "width", transform->size.x);
		sprite.height = tb_ini_query_float(element.start, NULL, "height", transform->size.y);

		sprite.frame = FrostMatchVariantINI(element.start, NULL, "frame", variant, 0);

		char texture[APPLICATION_STR_LEN];
		tb_ini_query_string(element.start, NULL, "texture", texture, APPLICATION_STR_LEN);

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
