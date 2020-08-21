#ifndef INVENTORY_H
#define INVENTORY_H

#include "math/math.h"

typedef struct
{
	float cell_size;
	float padding;
	vec2i size;
	vec2 offset;
} Inventory;

void RenderInventory(Inventory* inv, vec2 pos, mat4 view_proj);

#endif /* !INVENTORY_H */
