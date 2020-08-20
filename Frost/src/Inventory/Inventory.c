#include "Inventory.h"

#include "Graphics/Renderer.h"

void RenderInventory(Inventory* inv, vec2 pos, mat4 view_proj)
{
	Primitives2DStart(view_proj.v);

	float width = inv->size.x * (inv->cell_size + inv->padding) + inv->padding;
	float height = inv->size.y * (inv->cell_size + inv->padding) + inv->padding;

	Primitives2DRenderRect(pos.x, pos.y, width, height, IGNIS_WHITE);

	for (float y = inv->padding; y < height; y += inv->padding + inv->cell_size)
		for (float x = inv->padding; x < width; x += inv->padding + inv->cell_size)
		{
			Primitives2DRenderRect(pos.x + x, pos.y + y, inv->cell_size, inv->cell_size, IGNIS_WHITE);
		}

	Primitives2DFlush();
}
