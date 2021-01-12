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

	float cell_size;
	float padding;

	IgnisTexture2D* item_atlas;

	InventoryCellID dragged;
	InventoryCellID hover;
} InventorySystemData;

static InventorySystemData inventory_data;

int InventorySystemInit(IgnisTexture2D* item_atlas, Camera* camera, float cell_size, float padding)
{
	inventory_data.camera = camera;
	inventory_data.item_atlas = item_atlas;
	inventory_data.cell_size = cell_size;
	inventory_data.padding = padding;

	InventoryCellIDReset(&inventory_data.dragged);
	InventoryCellIDReset(&inventory_data.hover);

	return 1;
}

static float InventorySystemGetCellOffset(int index)
{
	return index * (inventory_data.cell_size + inventory_data.padding) + inventory_data.padding;
}

static int InventoryInitCells(Inventory* inv)
{
	size_t size = ((size_t)inv->rows * (size_t)inv->cols);
	inv->cells = malloc(sizeof(InventoryCell) * size);

	if (inv->cells)
	{
		memset(inv->cells, NULL_ITEM, size);
		return 1;
	}
	return 0;
}

int InventoryCreate(Inventory* inv, vec2 pos, int rows, int cols)
{
	inv->size.x = InventorySystemGetCellOffset(cols);
	inv->size.y = InventorySystemGetCellOffset(rows);

	inv->cols = cols;
	inv->rows = rows;

	inv->pos = pos;

	return InventoryInitCells(inv);
}

int InventoryCreateAligned(Inventory* inv, InventoryHAlign h_align, InventoryVAlign v_align, int rows, int cols)
{
	inv->size.x = InventorySystemGetCellOffset(cols);
	inv->size.y = InventorySystemGetCellOffset(rows);

	inv->cols = cols;
	inv->rows = rows;

	switch (h_align)
	{
	case INV_HALIGN_LEFT:	inv->pos.x = -inventory_data.camera->size.x * 0.5f; break;
	case INV_HALIGN_CENTER: inv->pos.x = -inv->size.x * 0.5f; break;
	case INV_HALIGN_RIGHT:	inv->pos.x = (inventory_data.camera->size.x * 0.5f) - inv->size.x; break;
	default: inv->pos.x = 0.0f;
	}

	switch (v_align)
	{
	case INV_VALIGN_TOP:	inv->pos.y = (inventory_data.camera->size.y * 0.5f) - inv->size.y; break;
	case INV_VALIGN_CENTER: inv->pos.y = -inv->size.y * 0.5f; break;
	case INV_VALIGN_BOTTOM: inv->pos.y = -inventory_data.camera->size.y * 0.5f; break;
	default: inv->pos.y = 0.0f;
	}

	return InventoryInitCells(inv);
}

static int InventoryGetCellAt(Inventory* inv, vec2 pos)
{
	vec2 cell_offset = vec2_sub(pos, inv->pos);

	/* filter first padding (left or under first cells) */
	if (cell_offset.x < inventory_data.padding || cell_offset.y < inventory_data.padding) return -1;

	/* prevent falsely jumping to next row */
	if (cell_offset.x >= (inv->size.x - inventory_data.padding)) return -1;

	int col = (int)(cell_offset.x / (inventory_data.cell_size + inventory_data.padding));
	int row = (int)(cell_offset.y / (inventory_data.cell_size + inventory_data.padding));

	/* filter cells that would be out of bounds */
	if (row >= inv->rows || col >= inv->cols) return -1;

	/* filter padding between cells */
	if ((cell_offset.x < InventorySystemGetCellOffset(col)) ||
		(cell_offset.y < InventorySystemGetCellOffset(row)))
		return -1;

	return InventoryGetCellIndex(inv, row, col);
}

void InventoryUpdateSystem(Ecs* ecs, float deltatime)
{
	InventoryCellIDReset(&inventory_data.hover);

	vec2 mouse = CameraGetMousePos(inventory_data.camera, InputMousePositionVec2());

	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_INVENTORY);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Inventory* inv = EcsComponentMapIterValue(iter);

		if (vec2_inside(mouse, inv->pos, vec2_add(inv->pos, inv->size)) && inv->state != INVENTORY_CLOSED)
			InventoryCellIDSet(&inventory_data.hover, EcsComponentMapIterKey(iter), InventoryGetCellAt(inv, mouse));
	}

	if (InputMousePressed(MOUSE_BUTTON_LEFT) && inventory_data.dragged.cell_index < 0)
		InventoryCellIDSet(&inventory_data.dragged, inventory_data.hover.entity, inventory_data.hover.cell_index);

	if (InputMouseReleased(MOUSE_BUTTON_LEFT) && inventory_data.dragged.cell_index >= 0)
	{
		Inventory* inv_hover = EcsGetDataComponent(ecs, inventory_data.hover.entity, COMPONENT_INVENTORY);
		Inventory* inv_dragged = EcsGetDataComponent(ecs, inventory_data.dragged.entity, COMPONENT_INVENTORY);

		if (inventory_data.hover.cell_index >= 0 && inventory_data.hover.cell_index < (inv_hover->rows * inv_hover->cols)
			&& inv_hover->cells[inventory_data.hover.cell_index].itemID == NULL_ITEM)
			InventoryMoveCellContent(inv_hover, inventory_data.hover.cell_index, inv_dragged, inventory_data.dragged.cell_index);

		InventoryCellIDReset(&inventory_data.dragged);
	}
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

		Primitives2DFillRect(inv->pos.x, inv->pos.y, inv->size.x, inv->size.y, bg);
		Primitives2DRenderRect(inv->pos.x, inv->pos.y, inv->size.x, inv->size.y, IGNIS_WHITE);

		for (int cell_index = 0; cell_index < (inv->rows * inv->cols); ++cell_index)
		{
			vec2 cell_pos = InventoryGetCellPos(inv, cell_index, inventory_data.cell_size, inventory_data.padding);

			Primitives2DRenderRect(cell_pos.x, cell_pos.y, inventory_data.cell_size, inventory_data.cell_size, IGNIS_WHITE);

			if (EcsComponentMapIterKey(iter) == inventory_data.hover.entity && cell_index == inventory_data.hover.cell_index)
				Primitives2DFillRect(cell_pos.x, cell_pos.y, inventory_data.cell_size, inventory_data.cell_size, IGNIS_WHITE);
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
			if (inv->cells[cell_index].itemID <= NULL_ITEM 
				|| (EcsComponentMapIterKey(iter) == inventory_data.dragged.entity 
				&& cell_index == inventory_data.dragged.cell_index))
				continue;

			vec2 cell_pos = InventoryGetCellPos(inv, cell_index, inventory_data.cell_size, inventory_data.padding);

			float src_x, src_y, src_w, src_h;
			GetTexture2DSrcRect(inventory_data.item_atlas, inv->cells[cell_index].itemID, &src_x, &src_y, &src_w, &src_h);

			BatchRenderer2DRenderTextureFrame(inventory_data.item_atlas, cell_pos.x, cell_pos.y, inventory_data.cell_size, inventory_data.cell_size, src_x, src_y, src_w, src_h);
		}
	}

	/* render dragged item */
	if (inventory_data.dragged.cell_index >= 0)
	{
		Inventory* inv = EcsGetDataComponent(ecs, inventory_data.dragged.entity, COMPONENT_INVENTORY);

		vec2 mouse_pos = CameraGetMousePos(inventory_data.camera, InputMousePositionVec2());
		float cell_x = mouse_pos.x - (inventory_data.cell_size / 2.0f);
		float cell_y = mouse_pos.y - (inventory_data.cell_size / 2.0f);

		float src_x, src_y, src_w, src_h;
		GetTexture2DSrcRect(inventory_data.item_atlas, inv->cells[inventory_data.dragged.cell_index].itemID, &src_x, &src_y, &src_w, &src_h);

		BatchRenderer2DRenderTextureFrame(inventory_data.item_atlas, cell_x, cell_y, inventory_data.cell_size, inventory_data.cell_size, src_x, src_y, src_w, src_h);
	}

	BatchRenderer2DFlush();
}