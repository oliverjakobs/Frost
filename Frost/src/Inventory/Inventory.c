#include "Inventory.h"

#include "Graphics/Renderer.h"

void RenderInventory(Inventory* inv, vec2 pos, mat4 view_proj)
{
	Primitives2DStart(view_proj.v);

	float x = pos.x + inv->offset.x;
	float y = pos.y + inv->offset.y;
	float width = inv->size.x * (inv->cell_size + inv->padding) + inv->padding;
	float height = inv->size.y * (inv->cell_size + inv->padding) + inv->padding;

	IgnisColorRGBA bg = IGNIS_WHITE;
	ignisBlendColorRGBA(&bg, 0.4f);
	Primitives2DFillRect(x, y, width, height, bg);
	Primitives2DRenderRect(x, y, width, height, IGNIS_WHITE);

	for (float offset_y = inv->padding; offset_y < height; offset_y += inv->padding + inv->cell_size)
		for (float offset_x = inv->padding; offset_x < width; offset_x += inv->padding + inv->cell_size)
		{
			Primitives2DRenderRect(x + offset_x, y + offset_y, inv->cell_size, inv->cell_size, IGNIS_WHITE);
		}

	Primitives2DFlush();
}
