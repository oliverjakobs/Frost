#include "Inventory.h"

#include "Graphics/Renderer.h"
#include "Application/Input.h"

int InventoryInit(Inventory* inv, vec2 pos, int rows, int columns, float cell_size, float padding)
{
	inv->cells = malloc(sizeof(InventoryCell) * rows * columns);

	if (!inv->cells) return 0;

	inv->pos = pos;
	inv->size.x = columns * (cell_size + padding) + padding;
	inv->size.y = rows * (cell_size + padding) + padding;

	inv->columns = columns;
	inv->rows = rows;

	inv->cell_size = cell_size;
	inv->padding = padding;

	/* initialize cells */
	for (int row = 0; row < rows; ++row)
		for (int col = 0; col < columns; ++col)
		{
			int index = InventoryGetCellIndex(inv, row, col);

			inv->cells[index].itemID = NULL_ITEM;
			inv->cells[index].pos.x = col * (cell_size + padding) + padding;
			inv->cells[index].pos.y = row * (cell_size + padding) + padding;
		}

	return 1;
}

void InventoryFree(Inventory* inv)
{
	if (inv->cells) free(inv->cells);
	inv->cells = NULL;

	inv->pos = vec2_zero();
	inv->size = vec2_zero();

	inv->columns = inv->rows = 0;

	inv->cell_size = 0.0f;
	inv->padding = 0.0f;
}

int InventoryGetCellIndex(Inventory* inv, int row, int column)
{
	return row * inv->columns + column;
}

int InventoryGetCellAt(Inventory* inv, vec2 pos)
{
	vec2 cell_offset = vec2_sub(pos, inv->pos);

	/* filter first padding (left or under first cells) */
	if (cell_offset.x < inv->padding || cell_offset.y < inv->padding) return -1;

	/* prevent falsely jumping to next row */
	if (cell_offset.x >= (inv->size.x - inv->padding)) return -1;

	int col = (int)(cell_offset.x / (inv->cell_size + inv->padding));
	int row = (int)(cell_offset.y / (inv->cell_size + inv->padding));

	/* filter cells that would be out of bounds */
	if (row >= inv->rows || col >= inv->columns) return -1;

	/* filter padding between cells */
	if ((cell_offset.x < col * (inv->cell_size + inv->padding) + inv->padding) ||
		(cell_offset.y < row * (inv->cell_size + inv->padding) + inv->padding))
		return -1;

	return InventoryGetCellIndex(inv, row, col);
}

void InventorySetCellContent(Inventory* inv, int index, int itemID)
{
	if (index < 0 || index >= (inv->rows * inv->columns)) return;

	inv->cells[index].itemID = itemID;
}

int InventoryGetCellContent(Inventory* inv, int index)
{
	if (index < 0 || index >= (inv->rows * inv->columns)) return NULL_ITEM;

	return inv->cells[index].itemID;
}

void InventoryMoveCellContent(Inventory* dst_inv, int dst_cell, Inventory* src_inv, int src_cell)
{
	InventorySetCellContent(dst_inv, dst_cell, InventoryGetCellContent(src_inv, src_cell));
	InventorySetCellContent(src_inv, src_cell, NULL_ITEM);
}

typedef struct
{
	int inv_index;
	int cell_index;
} InventoryCellID;

static void InventoryCellIDSet(InventoryCellID* id, int inv, int cell)
{
	id->inv_index = inv;
	id->cell_index = cell;
}

static void InventoryCellIDReset(InventoryCellID* id)
{
	id->inv_index = -1;
	id->cell_index = -1;
}

static InventoryCellID dragged;
static InventoryCellID hover;

void InventoryUpdateSystem(Inventory* invs, size_t count, Camera* camera, float deltatime)
{
	InventoryCellIDReset(&hover);

	vec2 mouse = CameraGetMousePos(camera, InputMousePositionVec2());

	for (int i = 0; i < count; ++i)
	{
		Inventory* inv = &invs[i];

		if (vec2_inside(mouse, inv->pos, vec2_add(inv->pos, inv->size)))
			InventoryCellIDSet(&hover, i, InventoryGetCellAt(inv, mouse));
	}

	if (InputMousePressed(MOUSE_BUTTON_LEFT) && dragged.cell_index < 0)
		InventoryCellIDSet(&dragged, hover.inv_index, hover.cell_index);

	if (InputMouseReleased(MOUSE_BUTTON_LEFT) && dragged.cell_index >= 0)
	{
		Inventory* inv = &invs[hover.inv_index];

		if (hover.cell_index >= 0 && hover.cell_index < (inv->rows * inv->columns) && inv->cells[hover.cell_index].itemID == NULL_ITEM)
			InventoryMoveCellContent(&invs[hover.inv_index], hover.cell_index, &invs[dragged.inv_index], dragged.cell_index);

		InventoryCellIDReset(&dragged);
	}
}

void InventoryRenderSystem(Inventory* invs, size_t count, IgnisTexture2D* item_atlas, Camera* camera)
{
	/* render inventory backgrounds */
	Primitives2DStart(CameraGetProjectionPtr(camera));

	IgnisColorRGBA bg = IGNIS_WHITE;
	ignisBlendColorRGBA(&bg, 0.4f);

	for (int inv_index = 0; inv_index < count; ++inv_index)
	{
		Inventory* inv = &invs[inv_index];

		Primitives2DFillRect(inv->pos.x, inv->pos.y, inv->size.x, inv->size.y, bg);
		Primitives2DRenderRect(inv->pos.x, inv->pos.y, inv->size.x, inv->size.y, IGNIS_WHITE);

		for (int cell_index = 0; cell_index < (inv->rows * inv->columns); ++cell_index)
		{
			vec2 cell_pos = vec2_add(inv->pos, inv->cells[cell_index].pos);

			Primitives2DRenderRect(cell_pos.x, cell_pos.y, inv->cell_size, inv->cell_size, IGNIS_WHITE);

			if (inv_index == hover.inv_index && cell_index == hover.cell_index)
				Primitives2DFillRect(cell_pos.x, cell_pos.y, inv->cell_size, inv->cell_size, IGNIS_WHITE);
		}
	}

	Primitives2DFlush();


	/* render inventory contents */
	BatchRenderer2DStart(CameraGetProjectionPtr(camera));

	for (int inv_index = 0; inv_index < count; ++inv_index)
	{
		Inventory* inv = &invs[inv_index];

		for (int cell_index = 0; cell_index < (inv->rows * inv->columns); ++cell_index)
		{
			if (inv->cells[cell_index].itemID <= NULL_ITEM || (inv_index == dragged.inv_index && cell_index == dragged.cell_index)) continue;

			vec2 cell_pos = vec2_add(inv->pos, inv->cells[cell_index].pos);

			float src_x, src_y, src_w, src_h;
			GetTexture2DSrcRect(item_atlas, inv->cells[cell_index].itemID, &src_x, &src_y, &src_w, &src_h);

			BatchRenderer2DRenderTextureFrame(item_atlas, cell_pos.x, cell_pos.y, inv->cell_size, inv->cell_size, src_x, src_y, src_w, src_h);
		}
	}

	/* render dragged item */
	if (dragged.cell_index >= 0)
	{
		Inventory* inv = &invs[dragged.inv_index];

		vec2 mouse_pos = CameraGetMousePos(camera, InputMousePositionVec2());
		float cell_x = mouse_pos.x - (inv->cell_size / 2.0f);
		float cell_y = mouse_pos.y - (inv->cell_size / 2.0f);

		float src_x, src_y, src_w, src_h;
		GetTexture2DSrcRect(item_atlas, inv->cells[dragged.cell_index].itemID, &src_x, &src_y, &src_w, &src_h);

		BatchRenderer2DRenderTextureFrame(item_atlas, cell_x, cell_y, inv->cell_size, inv->cell_size, src_x, src_y, src_w, src_h);
	}

	BatchRenderer2DFlush();
}

