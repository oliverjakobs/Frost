#include "RenderSystem.h"

#include "Frost/FrostEcs.h"
#include "Graphics/Renderer.h"

void RenderSystem(Ecs* ecs, const Camera* camera)
{
	BatchRenderer2DStart(CameraGetViewProjectionPtr(camera));

	COMPONENT_LIST_ITERATE(EcsGetComponentList(ecs, COMPONENT_Z_INDEX), i)
	{
		ZIndex* indexed = EcsGetOrderComponent(ecs, i, COMPONENT_Z_INDEX);
		Sprite* sprite = EcsGetDataComponent(ecs, indexed->entity, COMPONENT_SPRITE);

		if (!sprite) continue;

		vec2 pos = GetEntityPosition(ecs, indexed->entity);

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
