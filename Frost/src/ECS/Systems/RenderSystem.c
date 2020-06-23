#include "RenderSystem.h"

#include "Graphics/Renderer.h"

void RenderSystem(Ecs* ecs, ComponentTable* components, const float* mat_view_proj)
{
	BatchRenderer2DStart(mat_view_proj);

	for (size_t i = 0; i < ecs->indexed_entities.used; ++i)
	{
		ZIndexedEntity* indexed = clib_array_get(&ecs->indexed_entities, i);
		Sprite* sprite = ComponentTableGetComponent(components, indexed->entity, COMPONENT_SPRITE);

		if (!sprite) continue;

		vec2 pos = EntityGetPosition(indexed->entity, components);

		float x = pos.x - sprite->width / 2.0f;
		float y = pos.y;

		float src_w = 1.0f / sprite->texture->columns;
		float src_h = 1.0f / sprite->texture->rows;

		float src_x = (sprite->frame % sprite->texture->columns) * src_w;
		float src_y = 1 - src_h - ((sprite->frame / sprite->texture->columns) * src_h);

		switch (sprite->flip)
		{
		case SPRITE_FLIP_NONE:
			BatchRenderer2DRenderTextureFrame(sprite->texture, x, y, sprite->width, sprite->height, src_x, src_y, src_w, src_h);
			break;
		case SPRITE_FLIP_HORIZONTAL:
			BatchRenderer2DRenderTextureFrame(sprite->texture, x, y, sprite->width, sprite->height, src_x + src_w, src_y, -src_w, src_h);
			break;
		case SPRITE_FLIP_VERTICAL:
			BatchRenderer2DRenderTextureFrame(sprite->texture, x, y, sprite->width, sprite->height, src_x, src_y + src_h, src_w, -src_h);
			break;
		case SPRITE_FLIP_BOTH:
			BatchRenderer2DRenderTextureFrame(sprite->texture, x, y, sprite->width, sprite->height, src_x + src_w, src_y + src_h, -src_w, -src_h);
			break;
		}
	}

	BatchRenderer2DFlush();
}
