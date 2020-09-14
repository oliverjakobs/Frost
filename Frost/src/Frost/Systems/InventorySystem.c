#include "InventorySystem.h"

#include "Frost/FrostEcs.h"

typedef struct
{
	vec2 screen_size;

	float cell_size;
	float padding;

	IgnisTexture2D* item_atlas;
} InventorySystemData;

static InventorySystemData inventory_data;

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

static InventoryCellID dragged;
static InventoryCellID hover;

void InventorySystemLoad(IgnisTexture2D* item_atlas, vec2 screen_size, float cell_size, float padding)
{
	inventory_data.screen_size = screen_size;
	inventory_data.item_atlas = item_atlas;
	inventory_data.cell_size = cell_size;
	inventory_data.padding = padding;

	InventoryCellIDReset(&dragged);
	InventoryCellIDReset(&hover);
}

static vec2 InventorySystemGetMousePos(vec2 mouse)
{
	vec2 pos;
	pos.x = mouse.x - (inventory_data.screen_size.x / 2.0f);
	pos.y = (inventory_data.screen_size.y - mouse.y) - (inventory_data.screen_size.y / 2.0f);

	return pos;
}

static float InventorySystemGetCellOffset(int index)
{
	return index * (inventory_data.cell_size + inventory_data.padding) + inventory_data.padding;;
}

int InventoryInit(Inventory* inv, vec2 pos, int rows, int columns)
{
	inv->cells = malloc(sizeof(InventoryCell) * rows * columns);

	if (!inv->cells) return 0;

	inv->pos = pos;
	inv->size.x = InventorySystemGetCellOffset(columns);
	inv->size.y = InventorySystemGetCellOffset(rows);

	inv->columns = columns;
	inv->rows = rows;

	/* initialize cells */
	for (int row = 0; row < rows; ++row)
		for (int col = 0; col < columns; ++col)
		{
			int index = InventoryGetCellIndex(inv, row, col);

			inv->cells[index].itemID = NULL_ITEM;
			inv->cells[index].pos.x = InventorySystemGetCellOffset(col);
			inv->cells[index].pos.y = InventorySystemGetCellOffset(row);
		}

	return 1;
}

int InventoryInitAligned(Inventory* inv, InventoryHorizontalAlign h_align, InventoryVerticalAlign v_align, int rows, int columns)
{
	vec2 pos = vec2_zero();
	float w = InventorySystemGetCellOffset(columns);
	float h = InventorySystemGetCellOffset(rows);

	switch (h_align)
	{
	case INV_HALIGN_LEFT: pos.x = -inventory_data.screen_size.x * 0.5f; break;
	case INV_HALIGN_CENTER: pos.x = -w * 0.5f; break;
	case INV_HALIGN_RIGHT: pos.x = (inventory_data.screen_size.x * 0.5f) - w; break;
	}

	switch (v_align)
	{
	case INV_VALIGN_TOP: pos.y = (inventory_data.screen_size.y * 0.5f) - h; break;
	case INV_VALIGN_CENTER: pos.y = -h * 0.5f; break;
	case INV_VALIGN_BOTTOM: pos.y = -inventory_data.screen_size.y * 0.5f; break;
	}

	return InventoryInit(inv, pos, rows, columns);
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
	if (row >= inv->rows || col >= inv->columns) return -1;

	/* filter padding between cells */
	if ((cell_offset.x < InventorySystemGetCellOffset(col)) ||
		(cell_offset.y < InventorySystemGetCellOffset(row)))
		return -1;

	return InventoryGetCellIndex(inv, row, col);
}

void InventoryUpdateSystem(Ecs* ecs, float deltatime)
{
	InventoryCellIDReset(&hover);

	vec2 mouse = InventorySystemGetMousePos(InputMousePositionVec2());

	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_INVENTORY);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Inventory* inv = EcsComponentMapIterValue(iter);

		if (vec2_inside(mouse, inv->pos, vec2_add(inv->pos, inv->size)))
			InventoryCellIDSet(&hover, EcsComponentMapIterKey(iter), InventoryGetCellAt(inv, mouse));
	}

	if (InputMousePressed(MOUSE_BUTTON_LEFT) && dragged.cell_index < 0)
		InventoryCellIDSet(&dragged, hover.entity, hover.cell_index);

	if (InputMouseReleased(MOUSE_BUTTON_LEFT) && dragged.cell_index >= 0)
	{
		Inventory* inv_hover = EcsGetDataComponent(ecs, hover.entity, COMPONENT_INVENTORY);
		Inventory* inv_dragged = EcsGetDataComponent(ecs, dragged.entity, COMPONENT_INVENTORY);

		if (hover.cell_index >= 0 && hover.cell_index < (inv_hover->rows * inv_hover->columns)
			&& inv_hover->cells[hover.cell_index].itemID == NULL_ITEM)
			InventoryMoveCellContent(inv_hover, hover.cell_index, inv_dragged, dragged.cell_index);

		InventoryCellIDReset(&dragged);
	}
}

void InventoryRenderSystem(Ecs* ecs, const float* mat_view_proj)
{
	/* render inventory backgrounds */
	Primitives2DStart(mat_view_proj);

	IgnisColorRGBA bg = IGNIS_WHITE;
	ignisBlendColorRGBA(&bg, 0.4f);

	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_INVENTORY);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Inventory* inv = EcsComponentMapIterValue(iter);

		Primitives2DFillRect(inv->pos.x, inv->pos.y, inv->size.x, inv->size.y, bg);
		Primitives2DRenderRect(inv->pos.x, inv->pos.y, inv->size.x, inv->size.y, IGNIS_WHITE);

		for (int cell_index = 0; cell_index < (inv->rows * inv->columns); ++cell_index)
		{
			vec2 cell_pos = vec2_add(inv->pos, inv->cells[cell_index].pos);

			Primitives2DRenderRect(cell_pos.x, cell_pos.y, inventory_data.cell_size, inventory_data.cell_size, IGNIS_WHITE);

			if (EcsComponentMapIterKey(iter) == hover.entity && cell_index == hover.cell_index)
				Primitives2DFillRect(cell_pos.x, cell_pos.y, inventory_data.cell_size, inventory_data.cell_size, IGNIS_WHITE);
		}
	}

	Primitives2DFlush();

	/* render inventory contents */
	BatchRenderer2DStart(mat_view_proj);

	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Inventory* inv = EcsComponentMapIterValue(iter);

		for (int cell_index = 0; cell_index < (inv->rows * inv->columns); ++cell_index)
		{
			if (inv->cells[cell_index].itemID <= NULL_ITEM 
				|| (EcsComponentMapIterKey(iter) == dragged.entity && cell_index == dragged.cell_index))
				continue;

			vec2 cell_pos = vec2_add(inv->pos, inv->cells[cell_index].pos);

			float src_x, src_y, src_w, src_h;
			GetTexture2DSrcRect(inventory_data.item_atlas, inv->cells[cell_index].itemID, &src_x, &src_y, &src_w, &src_h);

			BatchRenderer2DRenderTextureFrame(inventory_data.item_atlas, cell_pos.x, cell_pos.y, inventory_data.cell_size, inventory_data.cell_size, src_x, src_y, src_w, src_h);
		}
	}

	/* render dragged item */
	if (dragged.cell_index >= 0)
	{
		Inventory* inv = EcsGetDataComponent(ecs, dragged.entity, COMPONENT_INVENTORY);

		vec2 mouse_pos = InventorySystemGetMousePos(InputMousePositionVec2());
		float cell_x = mouse_pos.x - (inventory_data.cell_size / 2.0f);
		float cell_y = mouse_pos.y - (inventory_data.cell_size / 2.0f);

		float src_x, src_y, src_w, src_h;
		GetTexture2DSrcRect(inventory_data.item_atlas, inv->cells[dragged.cell_index].itemID, &src_x, &src_y, &src_w, &src_h);

		BatchRenderer2DRenderTextureFrame(inventory_data.item_atlas, cell_x, cell_y, inventory_data.cell_size, inventory_data.cell_size, src_x, src_y, src_w, src_h);
	}

	BatchRenderer2DFlush();
}