#include "RenderSystem.h"
#include "Graphics/Renderer.h"

void EcsSystemRenderPre(const float* mat_view_proj)
{
	BatchRenderer2DStart(mat_view_proj);
}

void EcsSystemRender(EcsEntity* entity, ComponentTable* components)
{
	EcsTextureComponent* texture = (EcsTextureComponent*)ComponentTableGetComponent(components, entity->name, COMPONENT_TEXTURE);
	
	if (!texture) return;

	vec2 pos = EcsEntityGetPosition(entity->name, components);

	float x = pos.x - texture->width / 2.0f;
	float y = pos.y;

	float src_w = 1.0f / texture->texture->columns;
	float src_h = 1.0f / texture->texture->rows;

	float src_x = (texture->frame % texture->texture->columns) * src_w;
	float src_y = 1 - src_h - ((texture->frame / texture->texture->columns) * src_h);

	switch (texture->render_flip)
	{
	case RENDER_FLIP_NONE:
		BatchRenderer2DRenderTextureFrame(texture->texture, x, y, texture->width, texture->height, src_x, src_y, src_w, src_h);
		break;
	case RENDER_FLIP_HORIZONTAL:
		BatchRenderer2DRenderTextureFrame(texture->texture, x, y, texture->width, texture->height, src_x + src_w, src_y, -src_w, src_h);
		break;
	case RENDER_FLIP_VERTICAL:
		BatchRenderer2DRenderTextureFrame(texture->texture, x, y, texture->width, texture->height, src_x, src_y + src_h, src_w, -src_h);
		break;
	case RENDER_FLIP_BOTH:
		BatchRenderer2DRenderTextureFrame(texture->texture, x, y, texture->width, texture->height, src_x + src_w, src_y + src_h, -src_w, -src_h);
		break;
	}
}

void EcsSystemRenderPost()
{
	BatchRenderer2DFlush();
}
