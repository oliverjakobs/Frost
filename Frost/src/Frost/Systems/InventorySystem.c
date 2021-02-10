#include "InventorySystem.h"

#include "Frost/FrostEcs.h"
#include "Scenes/SceneLoader.h"

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
	vec2 mouse = CameraGetMousePos(&scene->camera, InputMousePositionVec2());

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
	if (InputMousePressed(MOUSE_BUTTON_LEFT) && dragged.cell < 0)
	{
		InventoryCellIDSet(&dragged, hover.entity, hover.cell);
	}
	else if (InputMouseReleased(MOUSE_BUTTON_LEFT) && inv_dragged && dragged.cell >= 0)
	{
		if (inv_hover && InventoryGetCellContent(inv_hover, hover.cell) == NULL_ITEM)
		{
			InventoryMoveCellContent(inv_hover, hover.cell, inv_dragged, dragged.cell);
		}
		else if (!inv_hover && InventoryGetCellContent(inv_dragged, dragged.cell) != NULL_ITEM)
		{
			vec2 drop_pos = CameraGetMousePosView(&scene->camera, InputMousePositionVec2());
			if (SceneLoadTemplate(scene, "item", drop_pos, 0, inv_dragged->cells[dragged.cell]))
				InventorySetCellContent(inv_dragged, dragged.cell, NULL_ITEM);
		}

		InventoryCellIDReset(&dragged);
	}
}

void InventoryRenderSystem(const Ecs* ecs, const Scene* scene, const float* mat_view_proj)
{
	/* render inventory backgrounds */
	/* TODO: background texture */
	Primitives2DStart(mat_view_proj);

	IgnisColorRGBA bg = IGNIS_WHITE;
	ignisBlendColorRGBA(&bg, 0.4f);

	EcsMap* map = EcsGetComponentMap(ecs, COMPONENT_INVENTORY);
	for (EcsMapIter* iter = EcsMapIterator(map); iter; iter = EcsMapIterNext(map, iter))
	{
		Inventory* inv = EcsMapIterValue(iter);
		if (inv->state == INVENTORY_CLOSED) continue;

		float width = InventoryGetWidth(inv);
		float height = InventoryGetHeight(inv);

		Primitives2DFillRect(inv->pos.x, inv->pos.y, width, height, bg);
		Primitives2DRenderRect(inv->pos.x, inv->pos.y, width, height, IGNIS_WHITE);

		for (int cell_index = 0; cell_index < (inv->rows * inv->cols); ++cell_index)
		{
			vec2 cell_pos = InventoryGetCellPos(inv, cell_index);

			Primitives2DRenderRect(cell_pos.x, cell_pos.y, inv->cell_size, inv->cell_size, IGNIS_WHITE);

			if (EcsMapIterKey(iter) == hover.entity && cell_index == hover.cell)
				Primitives2DFillRect(cell_pos.x, cell_pos.y, inv->cell_size, inv->cell_size, IGNIS_WHITE);
		}
	}

	Primitives2DFlush();

	/* render inventory contents */
	BatchRenderer2DStart(mat_view_proj);

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
			float size = inv->cell_size;
			size_t frame = inv->cells[cell];
			BatchRenderer2DRenderTextureFrame(scene->item_atlas, pos.x, pos.y, size, size, frame);
		}
	}

	/* render dragged item */
	if (dragged_inv && dragged.cell >= 0)
	{
		vec2 mouse_pos = CameraGetMousePos(&scene->camera, InputMousePositionVec2());
		float size = dragged_inv->cell_size;
		float x = mouse_pos.x - (size * 0.5f);
		float y = mouse_pos.y - (size * 0.5f);
		size_t frame = dragged_inv->cells[dragged.cell];

		BatchRenderer2DRenderTextureFrame(scene->item_atlas, x, y, size, size, frame);
	}

	BatchRenderer2DFlush();
}