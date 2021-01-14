#include "InventorySystem.h"

#include "Frost/FrostEcs.h"

typedef struct
{
	EcsEntityID entity;
	int cell_index;
} InventoryCellID;

static void InventoryCellIDSet(InventoryCellID* id, EcsEntityID entity, int cell)
{
	id->entity = entity;
	id->cell_index = cell;
}

static void InventoryCellIDReset(InventoryCellID* id)
{
	id->entity = ECS_NULL_ENTITY;
	id->cell_index = -1;
}

typedef struct
{
	Camera* camera;

	IgnisTexture2D* item_atlas;

} InventorySystemData;

static InventorySystemData inventory_data;

static InventoryCellID dragged;
static InventoryCellID hover;

int InventorySystemInit(IgnisTexture2D* item_atlas, Camera* camera)
{
	inventory_data.item_atlas = item_atlas;
	inventory_data.camera = camera;

	InventoryCellIDReset(&dragged);
	InventoryCellIDReset(&hover);

	return 1;
}

void InventoryUpdateSystem(Ecs* ecs, float deltatime)
{
	InventoryCellIDReset(&hover);

	vec2 mouse = CameraGetMousePos(inventory_data.camera, InputMousePositionVec2());

	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_INVENTORY);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Inventory* inv = EcsComponentMapIterValue(iter);

		vec2 size;
		size.x = InventoryGetWidth(inv);
		size.y = InventoryGetHeight(inv);

		if (vec2_inside(mouse, inv->pos, vec2_add(inv->pos, size)) && inv->state != INVENTORY_CLOSED)
		{
			int cell = InventoryGetCellAt(inv, mouse);
			InventoryCellIDSet(&hover, EcsComponentMapIterKey(iter), cell);
		}
	}

	if (InputMousePressed(MOUSE_BUTTON_LEFT) && dragged.cell_index < 0)
		InventoryCellIDSet(&dragged, hover.entity, hover.cell_index);

	if (InputMouseReleased(MOUSE_BUTTON_LEFT) && dragged.cell_index >= 0)
	{
		Inventory* inv_hover = EcsGetDataComponent(ecs, hover.entity, COMPONENT_INVENTORY);
		Inventory* inv_dragged = EcsGetDataComponent(ecs, dragged.entity, COMPONENT_INVENTORY);

		if (hover.cell_index >= 0 && hover.cell_index < (inv_hover->rows * inv_hover->cols)
			&& inv_hover->cells[hover.cell_index] == NULL_ITEM)
			InventoryMoveCellContent(inv_hover, hover.cell_index, inv_dragged, dragged.cell_index);

		InventoryCellIDReset(&dragged);
	}

	/* TODO drop items */
}

void InventoryRenderSystem(const Ecs* ecs, const float* mat_view_proj)
{
	/* render inventory backgrounds */
	Primitives2DStart(mat_view_proj);

	IgnisColorRGBA bg = IGNIS_WHITE;
	ignisBlendColorRGBA(&bg, 0.4f);

	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_INVENTORY);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Inventory* inv = EcsComponentMapIterValue(iter);
		if (inv->state == INVENTORY_CLOSED) continue;

		float width = InventoryGetWidth(inv);
		float height = InventoryGetHeight(inv);

		Primitives2DFillRect(inv->pos.x, inv->pos.y, width, height, bg);
		Primitives2DRenderRect(inv->pos.x, inv->pos.y, width, height, IGNIS_WHITE);

		for (int cell_index = 0; cell_index < (inv->rows * inv->cols); ++cell_index)
		{
			vec2 cell_pos = InventoryGetCellPos(inv, cell_index);

			Primitives2DRenderRect(cell_pos.x, cell_pos.y, inv->cell_size, inv->cell_size, IGNIS_WHITE);

			if (EcsComponentMapIterKey(iter) == hover.entity && cell_index == hover.cell_index)
				Primitives2DFillRect(cell_pos.x, cell_pos.y, inv->cell_size, inv->cell_size, IGNIS_WHITE);
		}
	}

	Primitives2DFlush();

	/* render inventory contents */
	BatchRenderer2DStart(mat_view_proj);

	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Inventory* inv = EcsComponentMapIterValue(iter);
		if (inv->state == INVENTORY_CLOSED) continue;

		for (int cell_index = 0; cell_index < (inv->rows * inv->cols); ++cell_index)
		{
			if (inv->cells[cell_index] == NULL_ITEM || (EcsComponentMapIterKey(iter) == dragged.entity 
				&& cell_index == dragged.cell_index))
				continue;

			vec2 cell_pos = InventoryGetCellPos(inv, cell_index);

			float src_x, src_y, src_w, src_h;
			GetTexture2DSrcRect(inventory_data.item_atlas, inv->cells[cell_index], &src_x, &src_y, &src_w, &src_h);

			BatchRenderer2DRenderTextureFrame(inventory_data.item_atlas, cell_pos.x, cell_pos.y, inv->cell_size, inv->cell_size, src_x, src_y, src_w, src_h);
		}
	}

	/* render dragged item */
	if (dragged.cell_index >= 0)
	{
		Inventory* inv = EcsGetDataComponent(ecs, dragged.entity, COMPONENT_INVENTORY);

		vec2 mouse_pos = CameraGetMousePos(inventory_data.camera, InputMousePositionVec2());
		float cell_x = mouse_pos.x - (inv->cell_size * 0.5f);
		float cell_y = mouse_pos.y - (inv->cell_size * 0.5f);

		float src_x, src_y, src_w, src_h;
		GetTexture2DSrcRect(inventory_data.item_atlas, inv->cells[dragged.cell_index], &src_x, &src_y, &src_w, &src_h);

		BatchRenderer2DRenderTextureFrame(inventory_data.item_atlas, cell_x, cell_y, inv->cell_size, inv->cell_size, src_x, src_y, src_w, src_h);
	}

	BatchRenderer2DFlush();
}