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

	inv->drag = NULL_CELL;
	inv->hover = NULL_CELL;

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

	inv->drag = NULL_CELL;
	inv->hover = NULL_CELL;
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

void InventoryUpdate(Inventory* inv, Camera* camera, float deltatime)
{
	vec2 mouse = CameraGetMousePos(camera, InputMousePositionVec2());

	if (vec2_inside(mouse, inv->pos, vec2_add(inv->pos, inv->size)))
		inv->hover = InventoryGetCellAt(inv, mouse);

	if (InputMousePressed(MOUSE_BUTTON_LEFT) && inv->drag <= NULL_CELL)
		inv->drag = inv->hover;

	if (InputMouseReleased(MOUSE_BUTTON_LEFT) && inv->drag > NULL_CELL)
	{
		if (inv->hover >= 0 && inv->hover < (inv->rows * inv->columns))
		{
			if (inv->cells[inv->hover].itemID == NULL_ITEM)
			{
				InventorySetCellContent(inv, inv->hover, InventoryGetCellContent(inv, inv->drag));
				InventorySetCellContent(inv, inv->drag, NULL_ITEM);
			}
		}

		inv->drag = -1;
	}
}

void InventoryRender(Inventory* inv, Camera* camera)
{
	Primitives2DStart(CameraGetProjectionPtr(camera));

	IgnisColorRGBA bg = IGNIS_WHITE;
	ignisBlendColorRGBA(&bg, 0.4f);
	Primitives2DFillRect(inv->pos.x, inv->pos.y, inv->size.x, inv->size.y, bg);
	Primitives2DRenderRect(inv->pos.x, inv->pos.y, inv->size.x, inv->size.y, IGNIS_WHITE);

	for (int i = 0; i < (inv->rows * inv->columns); ++i)
	{
		vec2 cell_pos = vec2_add(inv->pos, inv->cells[i].pos);

		Primitives2DRenderRect(cell_pos.x, cell_pos.y, inv->cell_size, inv->cell_size, IGNIS_WHITE);

		if (i == inv->hover)
			Primitives2DFillRect(cell_pos.x, cell_pos.y, inv->cell_size, inv->cell_size, IGNIS_WHITE);
	}

	Primitives2DFlush();
}

void InventoryRenderContent(Inventory* inv, IgnisTexture2D* item_atlas, Camera* camera)
{
	BatchRenderer2DStart(CameraGetProjectionPtr(camera));

	for (int i = 0; i < (inv->rows * inv->columns); ++i)
	{
		if (inv->cells[i].itemID <= NULL_ITEM || i == inv->drag) continue;

		vec2 cell_pos = vec2_add(inv->pos, inv->cells[i].pos);

		float src_x, src_y, src_w, src_h;
		GetTexture2DSrcRect(item_atlas, inv->cells[i].itemID, &src_x, &src_y, &src_w, &src_h);

		BatchRenderer2DRenderTextureFrame(item_atlas, cell_pos.x, cell_pos.y, inv->cell_size, inv->cell_size, src_x, src_y, src_w, src_h);
	}

	/* render dragged item */
	if (inv->drag > NULL_CELL && inv->cells[inv->drag].itemID)
	{
		vec2 mouse_pos = CameraGetMousePos(camera, InputMousePositionVec2());
		float cell_x = mouse_pos.x - (inv->cell_size / 2.0f);
		float cell_y = mouse_pos.y - (inv->cell_size / 2.0f);

		float src_x, src_y, src_w, src_h;
		GetTexture2DSrcRect(item_atlas, inv->cells[inv->drag].itemID, &src_x, &src_y, &src_w, &src_h);

		BatchRenderer2DRenderTextureFrame(item_atlas, cell_x, cell_y, inv->cell_size, inv->cell_size, src_x, src_y, src_w, src_h);
	}

	BatchRenderer2DFlush();
}

