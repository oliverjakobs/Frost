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

	inv->hover = -1;

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

	inv->hover = -1;
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

	int col = cell_offset.x / (inv->cell_size + inv->padding);
	int row = cell_offset.y / (inv->cell_size + inv->padding);

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

void InventoryUpdate(Inventory* inv, Camera* camera, float deltatime)
{
	vec2 mouse = CameraGetMousePos(camera, InputMousePositionVec2());

	if (vec2_inside(mouse, inv->pos, vec2_add(inv->pos, inv->size)))
	{
		inv->hover = InventoryGetCellAt(inv, mouse);

		if (InputMousePressed(MOUSE_BUTTON_LEFT))
			InventorySetCellContent(inv, inv->hover, 1);
	}
}

void InventoryRender(Inventory* inv, mat4 view_proj)
{
	Primitives2DStart(view_proj.v);

	IgnisColorRGBA bg = IGNIS_WHITE;
	ignisBlendColorRGBA(&bg, 0.4f);
	Primitives2DFillRect(inv->pos.x, inv->pos.y, inv->size.x, inv->size.y, bg);
	Primitives2DRenderRect(inv->pos.x, inv->pos.y, inv->size.x, inv->size.y, IGNIS_WHITE);

	for (int i = 0; i < (inv->rows * inv->columns); ++i)
	{
		float cell_x = inv->pos.x + inv->cells[i].pos.x;
		float cell_y = inv->pos.y + inv->cells[i].pos.y;

		Primitives2DRenderRect(cell_x, cell_y, inv->cell_size, inv->cell_size, IGNIS_WHITE);

		if (i == inv->hover)
			Primitives2DFillRect(cell_x, cell_y, inv->cell_size, inv->cell_size, IGNIS_WHITE);
	}

	Primitives2DFlush();

	/* render inventory content */
}

void InventoryRenderContent(Inventory* inv, IgnisTexture2D* item_atlas, mat4 view_proj)
{
	BatchRenderer2DStart(view_proj.v);

	for (int i = 0; i < (inv->rows * inv->columns); ++i)
	{
		float cell_x = inv->pos.x + inv->cells[i].pos.x;
		float cell_y = inv->pos.y + inv->cells[i].pos.y;

		if (inv->cells[i].itemID <= 0) continue;

		float src_x, src_y, src_w, src_h;
		GetTexture2DSrcRect(item_atlas, inv->cells[i].itemID, &src_x, &src_y, &src_w, &src_h);

		BatchRenderer2DRenderTextureFrame(item_atlas, cell_x, cell_y, inv->cell_size, inv->cell_size, src_x, src_y, src_w, src_h);
	}

	BatchRenderer2DFlush();
}

