#include "RenderSystem.h"

#include "Frost/FrostEcs.h"
#include "Graphics/Renderer.h"

void RenderSystem(Ecs* ecs, const float* mat_view_proj)
{
	BatchRenderer2DStart(mat_view_proj);

	EcsComponentList* list = EcsGetComponentList(ecs, COMPONENT_Z_INDEX);

	for (EcsListNode* it = list->first; it; it = EcsComponentNodeNext(it))
	{
		ZIndex* indexed = EcsComponentNodeComponent(it);
		Sprite* sprite = EcsGetDataComponent(ecs, EcsComponentNodeEntity(it), COMPONENT_SPRITE);

		if (!sprite) continue;

		vec2 pos = GetEntityPosition(ecs, EcsComponentNodeEntity(it));

		float x = pos.x - sprite->width / 2.0f;
		float y = pos.y;

		float src_x, src_y, src_w, src_h;
		GetTexture2DSrcRect(sprite->texture, sprite->frame, &src_x, &src_y, &src_w, &src_h);

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
