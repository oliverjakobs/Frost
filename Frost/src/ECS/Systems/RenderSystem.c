#include "RenderSystem.h"
#include "IgnisRenderer/IgnisRenderer.h"

void EcsSystemRender(EcsEntity* entity)
{
	if (!entity->texture) return;

	vec2 pos = EcsEntityGetPosition(entity);

	float x = pos.x - entity->texture->width / 2.0f;
	float y = pos.y;

	float src_w = 1.0f / entity->texture->texture->columns;
	float src_h = 1.0f / entity->texture->texture->rows;

	float src_x = (entity->texture->frame % entity->texture->texture->columns) * src_w;
	float src_y = 1 - src_h - ((entity->texture->frame / entity->texture->texture->columns) * src_h);

	switch (entity->texture->render_flip)
	{
	case RENDER_FLIP_NONE:
		BatchRenderer2DRenderTextureFrame(entity->texture->texture, x, y, entity->texture->width, entity->texture->height, src_x, src_y, src_w, src_h);
		break;
	case RENDER_FLIP_HORIZONTAL:
		BatchRenderer2DRenderTextureFrame(entity->texture->texture, x, y, entity->texture->width, entity->texture->height, src_x + src_w, src_y, -src_w, src_h);
		break;
	case RENDER_FLIP_VERTICAL:
		BatchRenderer2DRenderTextureFrame(entity->texture->texture, x, y, entity->texture->width, entity->texture->height, src_x, src_y + src_h, src_w, -src_h);
		break;
	case RENDER_FLIP_BOTH:
		BatchRenderer2DRenderTextureFrame(entity->texture->texture, x, y, entity->texture->width, entity->texture->height, src_x + src_w, src_y + src_h, -src_w, -src_h);
		break;
	}
}
