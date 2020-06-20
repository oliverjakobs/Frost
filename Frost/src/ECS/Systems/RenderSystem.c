#include "RenderSystem.h"

#include "Graphics/Renderer.h"

void EcsSystemRenderPre(const float* mat_view_proj)
{
	BatchRenderer2DStart(mat_view_proj);
}

void EcsSystemRender(ComponentTable* components, const char* entity)
{
	Sprite* sprite = ComponentTableGetComponent(components, entity, COMPONENT_TEXTURE);
	
	if (!sprite) return;

	vec2 pos = EntityGetPosition(entity, components);

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

void EcsSystemRenderPost()
{
	BatchRenderer2DFlush();
}
