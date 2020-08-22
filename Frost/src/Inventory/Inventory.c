#include "Inventory.h"

#include "Graphics/Renderer.h"
#include "Application/Input.h"

void InventoryInit(Inventory* inv, vec2 pos, int rows, int columns, float cell_size, float padding)
{
	inv->pos = pos;
	inv->size.x = columns * (cell_size + padding) + padding;
	inv->size.y = rows * (cell_size + padding) + padding;

	inv->columns = columns;
	inv->rows = rows;

	inv->cell_size = cell_size;
	inv->padding = padding;

	inv->hover = -1;
}

int InventoryGetCellIndex(Inventory* inv, vec2i pos)
{
	return pos.y * inv->columns + pos.x;
}

vec2i InventoryGetCellPos(Inventory* inv, int index)
{
	return vec2i_zero();
}

int InventoryGetCellAt(Inventory* inv, vec2 pos)
{
	vec2 rel_pos = vec2_sub(pos, inv->pos);

	if (rel_pos.x < inv->padding || rel_pos.y < inv->padding) return -1;

	if (rel_pos.x >= (inv->size.x - inv->padding)) return -1;

	int col = rel_pos.x / (inv->cell_size + inv->padding);
	int row = rel_pos.y / (inv->cell_size + inv->padding);

	if (row >= inv->rows || col >= inv->columns) return -1;

	/* filter padding between cells */
	if (rel_pos.x < col * (inv->cell_size + inv->padding) + inv->padding)
		return -1;

	if (rel_pos.y < row * (inv->cell_size + inv->padding) + inv->padding)
		return -1;

	return InventoryGetCellIndex(inv, (vec2i) {col, row});
}

void InventoryUpdate(Inventory* inv, Camera* camera, float deltatime)
{
	vec2 mouse = CameraGetMousePos(camera, InputMousePositionVec2());

	if (vec2_inside(mouse, inv->pos, vec2_add(inv->pos, inv->size)))
		inv->hover = InventoryGetCellAt(inv, mouse);
}

void InventoryRender(Inventory* inv, vec2 offset, mat4 view_proj)
{
	Primitives2DStart(view_proj.v);

	float x = inv->pos.x + offset.x;
	float y = inv->pos.y + offset.y;
	float width = inv->size.x;
	float height = inv->size.y;

	IgnisColorRGBA bg = IGNIS_WHITE;
	ignisBlendColorRGBA(&bg, 0.4f);
	Primitives2DFillRect(x, y, width, height, bg);
	Primitives2DRenderRect(x, y, width, height, IGNIS_WHITE);

	int index = 0;
	for (float cell_y = inv->padding; cell_y < height; cell_y += inv->padding + inv->cell_size)
		for (float cell_x = inv->padding; cell_x < width; cell_x += inv->padding + inv->cell_size)
		{
			Primitives2DRenderRect(x + cell_x, y + cell_y, inv->cell_size, inv->cell_size, IGNIS_WHITE);

			if (index++ == inv->hover)
			{
				Primitives2DFillRect(x + cell_x, y + cell_y, inv->cell_size, inv->cell_size, IGNIS_WHITE);
			}
		}

	Primitives2DFlush();
}
