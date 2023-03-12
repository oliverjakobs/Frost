#include "RenderSystem.h"

#include "Frost/Frost.h"
#include "tile/tile_renderer.h"

void RenderSystem(const Ecs* ecs, const Scene* scene, const float* mat_view_proj)
{
	Batch2DSetViewProjection(mat_view_proj);

	EcsList* list = EcsGetComponentList(ecs, COMPONENT_Z_INDEX);
	for (size_t index = 0; index < EcsListSize(list); ++index)
	{
		ZIndex* indexed = EcsListComponentAt(list, index);
		ECS_REQUIRE_LIST(Sprite, ecs, sprite, list, index, COMPONENT_SPRITE);

		vec2 pos = GetEntityPosition(ecs, EcsListEntityAt(list, index));

		float x = pos.x - sprite->width / 2.0f;
		float y = pos.y;
		float w = sprite->width;
		float h = sprite->height;

		float src_x, src_y, src_w, src_h;
		SpriteGetSrcRect(sprite, &src_x, &src_y, &src_w, &src_h);

		Batch2DRenderTextureSrc(sprite->texture, x, y, w, h, src_x, src_y, src_w, src_h);
	}

	Batch2DFlush();
}

void DebugRenderSystem(const Ecs* ecs, const Scene* scene, const float* mat_view_proj)
{
	Primitives2DSetViewProjection(mat_view_proj);

	EcsMap* map = EcsGetComponentMap(ecs, COMPONENT_TRANSFORM);
	for (EcsMapIter* iter = EcsMapIterator(map); iter; iter = EcsMapIterNext(map, iter))
	{
		Transform* transform = EcsMapIterValue(iter);
		Primitives2DRenderCircle(transform->position.x, transform->position.y, 2.0f, IGNIS_WHITE);
	}

	map = EcsGetComponentMap(ecs, COMPONENT_INTERACTABLE);
	for (EcsMapIter* iter = EcsMapIterator(map); iter; iter = EcsMapIterNext(map, iter))
	{
		Interactable* interactable = EcsMapIterValue(iter);
		vec2 cen = GetEntityCenter(ecs, EcsMapIterKey(iter));
		Primitives2DRenderCircle(cen.x, cen.y, interactable->range_min, IGNIS_WHITE);
		Primitives2DRenderCircle(cen.x, cen.y, interactable->range_max, IGNIS_WHITE);
	}

	map = EcsGetComponentMap(ecs, COMPONENT_RIGID_BODY);
	for (EcsMapIter* iter = EcsMapIterator(map); iter; iter = EcsMapIterNext(map, iter))
	{
		RigidBody* body = EcsMapIterValue(iter);
		TileBodyRenderDebug(&body->body);
	}

	Primitives2DFlush();
}
