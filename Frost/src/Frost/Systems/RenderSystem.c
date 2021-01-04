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
		ECS_COMPONENT_REQUIRE_NODE(Sprite, ecs, sprite, it, COMPONENT_SPRITE);

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

void DebugRenderSystem(Ecs* ecs, const float* mat_view_proj)
{
	Primitives2DStart(mat_view_proj);

	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_TRANSFORM);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Transform* transform = EcsComponentMapIterValue(iter);
		Primitives2DRenderCircle(transform->position.x, transform->position.y, 2.0f, IGNIS_WHITE);
	}

	map = EcsGetComponentMap(ecs, COMPONENT_INTERACTABLE);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Interactable* interactable = EcsComponentMapIterValue(iter);
		vec2 cen = GetEntityCenter(ecs, EcsComponentMapIterKey(iter));
		Primitives2DRenderCircle(cen.x, cen.y, interactable->min_radius, IGNIS_WHITE);
		Primitives2DRenderCircle(cen.x, cen.y, interactable->max_radius, IGNIS_WHITE);
	}

	map = EcsGetComponentMap(ecs, COMPONENT_RIGID_BODY);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		RigidBody* body = EcsComponentMapIterValue(iter);

		vec2 pos = vec2_sub(body->body.position, body->body.half_dim);
		vec2 dim = vec2_mult(body->body.half_dim, 2.0f);
		Primitives2DRenderRect(pos.x, pos.y, dim.x, dim.y, body->type == RIGID_BODY_DYNAMIC ? IGNIS_GREEN : IGNIS_WHITE);

		if (body->type == RIGID_BODY_DYNAMIC)
		{
			vec2 position = body->body.position;
			vec2 half_dim = body->body.half_dim;
			float tile_size = body->body.map->tile_size;
			float sensor_offset = body->body.sensor_offset;

			Primitives2DRenderRect(pos.x, pos.y, dim.x, dim.y, body->body.on_slope ? IGNIS_BLUE : IGNIS_WHITE);

			line sensor = TileBodyGetSensor(&body->body, TILE_BOTTOM, body->body.position);
			Primitives2DRenderLine(sensor.start.x, sensor.start.y, sensor.end.x, sensor.end.y, IGNIS_WHITE);

			sensor = TileBodyGetSensor(&body->body, TILE_TOP, body->body.position);
			Primitives2DRenderLine(sensor.start.x, sensor.start.y, sensor.end.x, sensor.end.y, IGNIS_WHITE);

			sensor = TileBodyGetSensor(&body->body, TILE_LEFT, body->body.position);
			Primitives2DRenderLine(sensor.start.x, sensor.start.y, sensor.end.x, sensor.end.y, IGNIS_WHITE);

			sensor = TileBodyGetSensor(&body->body, TILE_RIGHT, body->body.position);
			Primitives2DRenderLine(sensor.start.x, sensor.start.y, sensor.end.x, sensor.end.y, IGNIS_WHITE);
		}
		else
			Primitives2DRenderRect(pos.x, pos.y, dim.x, dim.y, IGNIS_WHITE);
	}

	Primitives2DFlush();
}
