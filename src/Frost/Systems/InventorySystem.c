#include "InventorySystem.h"

#include "Frost/Frost.h"
#include "Frost/SceneLoader.h"

typedef struct
{
	EcsEntityID entity;
	int cell;
} InventoryCellID;

static void InventoryCellIDSet(InventoryCellID* id, EcsEntityID entity, int cell)
{
	id->entity = entity;
	id->cell = cell;
}

static void InventoryCellIDReset(InventoryCellID* id)
{
	id->entity = ECS_NULL_ENTITY;
	id->cell = -1;
}

static InventoryCellID dragged = { ECS_NULL_ENTITY, -1 };
static InventoryCellID hover = { ECS_NULL_ENTITY, -1 };

void InventoryUpdateSystem(Ecs* ecs, Scene* scene, float deltatime)
{
	InventoryCellIDReset(&hover);

	Inventory* inv_hover = NULL;
	Inventory* inv_dragged = NULL;
	vec2 mouse = { 0 };
	minimalCursorPos(&mouse.x, &mouse.y);
	mouse = cameraGetMousePos(&scene->camera, mouse);

	/* update hover cell */
	EcsMap* map = EcsGetComponentMap(ecs, COMPONENT_INVENTORY);
	for (EcsMapIter* iter = EcsMapIterator(map); iter; iter = EcsMapIterNext(map, iter))
	{
		Inventory* inv = EcsMapIterValue(iter);

		if (EcsMapIterKey(iter) == dragged.entity) inv_dragged = inv;

		vec2 max;
		max.x = inv->pos.x + InventoryGetWidth(inv);
		max.y = inv->pos.y + InventoryGetHeight(inv);

		if (vec2_inside(mouse, inv->pos, max) && inv->state != INVENTORY_CLOSED)
		{
			int cell = InventoryGetCellAt(inv, mouse);
			InventoryCellIDSet(&hover, EcsMapIterKey(iter), cell);
			inv_hover = inv;
			break;
		}
	}

	/* update dragged item/cell */
	if (minimalMousePressed(MINIMAL_MOUSE_BUTTON_LEFT) && dragged.cell < 0)
	{
		InventoryCellIDSet(&dragged, hover.entity, hover.cell);
	}
	else if (minimalMouseReleased(MINIMAL_MOUSE_BUTTON_LEFT) && inv_dragged && dragged.cell >= 0)
	{
		if (inv_hover && InventoryGetCellContent(inv_hover, hover.cell) == NULL_ITEM)
		{
			InventoryMoveCellContent(inv_hover, hover.cell, inv_dragged, dragged.cell);
		}
		else if (!inv_hover && InventoryGetCellContent(inv_dragged, dragged.cell) != NULL_ITEM)
		{
			vec2 mouse = {0};
			minimalCursorPos(&mouse.x, &mouse.y);
			vec2 drop_pos = cameraGetMousePosView(&scene->camera, mouse);
			ItemID id = inv_dragged->cells[dragged.cell];
			EcsEntityID entity = EcsEntityGetNextID();
			if (SceneLoadTemplate(scene, entity, "res/templates/item.json", drop_pos, 0, id))
				InventorySetCellContent(inv_dragged, dragged.cell, NULL_ITEM);
		}

		InventoryCellIDReset(&dragged);
	}
}

void InventoryRenderSystem(const Ecs* ecs, const Scene* scene, const float* mat_view_proj)
{
	/* render inventory backgrounds */
	/* TODO: background texture */
	ignisPrimitivesRendererSetViewProjection(mat_view_proj);

	IgnisColorRGBA bg = IGNIS_WHITE;
	ignisBlendColorRGBA(&bg, 0.4f);

	EcsMap* map = EcsGetComponentMap(ecs, COMPONENT_INVENTORY);
	for (EcsMapIter* iter = EcsMapIterator(map); iter; iter = EcsMapIterNext(map, iter))
	{
		Inventory* inv = EcsMapIterValue(iter);
		if (inv->state == INVENTORY_CLOSED) continue;

		float width = InventoryGetWidth(inv);
		float height = InventoryGetHeight(inv);

		ignisPrimitives2DFillRect(inv->pos.x, inv->pos.y, width, height, bg);
		ignisPrimitives2DRenderRect(inv->pos.x, inv->pos.y, width, height, IGNIS_WHITE);

		for (int cell = 0; cell < (inv->rows * inv->cols); ++cell)
		{
			vec2 pos = InventoryGetCellPos(inv, cell);
			float size = inv->cell_size;

			ignisPrimitives2DRenderRect(pos.x, pos.y, size, size, IGNIS_WHITE);

			if (EcsMapIterKey(iter) == hover.entity && cell == hover.cell)
				ignisPrimitives2DFillRect(pos.x, pos.y, size, size, IGNIS_WHITE);
		}
	}

	ignisPrimitivesRendererFlush();

	/* render inventory contents */
	ignisBatch2DSetViewProjection(mat_view_proj);

	Inventory* dragged_inv = NULL;
	for (EcsMapIter* iter = EcsMapIterator(map); iter; iter = EcsMapIterNext(map, iter))
	{
		Inventory* inv = EcsMapIterValue(iter);
		if (inv->state == INVENTORY_CLOSED) continue;

		for (int cell = 0; cell < (inv->rows * inv->cols); ++cell)
		{
			if (inv->cells[cell] == NULL_ITEM) continue; /* skip empty cells */

			/* dragged item is rendered later */
			if (EcsMapIterKey(iter) == dragged.entity && cell == dragged.cell)
			{
				dragged_inv = inv;
				continue;
			}

			vec2 pos = InventoryGetCellPos(inv, cell);
			IgnisRect rect = {
				pos.x, pos.y,
				inv->cell_size, inv->cell_size
			};
			uint32_t frame = inv->cells[cell];
			uint32_t cols = scene->item_atlas_size.x;
			uint32_t rows = scene->item_atlas_size.y;
			IgnisRect src = ignisGetTexture2DSrcRect(scene->item_atlas, cols, rows, frame);

			ignisBatch2DRenderTextureSrc(scene->item_atlas, rect, src);
		}
	}

	/* render dragged item */
	if (dragged_inv && dragged.cell >= 0)
	{
		vec2 mouse_pos = { 0 };
		minimalCursorPos(&mouse_pos.x, &mouse_pos.y);
		mouse_pos = cameraGetMousePos(&scene->camera, mouse_pos);

		IgnisRect rect = {
			mouse_pos.x - (dragged_inv->cell_size * 0.5f),
			mouse_pos.y - (dragged_inv->cell_size * 0.5f),
			dragged_inv->cell_size,
			dragged_inv->cell_size
		};

		uint32_t frame = dragged_inv->cells[dragged.cell];
		uint32_t cols = scene->item_atlas_size.x;
		uint32_t rows = scene->item_atlas_size.y;
		IgnisRect src = ignisGetTexture2DSrcRect(scene->item_atlas, cols, rows, frame);

		ignisBatch2DRenderTextureSrc(scene->item_atlas, rect, src);
	}

	ignisBatch2DFlush();
}