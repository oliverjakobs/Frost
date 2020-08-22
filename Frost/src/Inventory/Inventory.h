#ifndef INVENTORY_H
#define INVENTORY_H

#include "Camera/Camera.h"

typedef struct
{
	vec2 pos;
	vec2 size;

	int rows;
	int columns;

	float cell_size;
	float padding;

	int hover;
} Inventory;

void InventoryInit(Inventory* inv, vec2 pos, int rows, int columns, float cell_size, float padding);

int InventoryGetCellIndex(Inventory* inv, vec2i pos);
vec2i InventoryGetCellPos(Inventory* inv, int index);

int InventoryGetCellAt(Inventory* inv, vec2 pos);

void InventoryUpdate(Inventory* inv, Camera* camera, float deltatime);
void InventoryRender(Inventory* inv, vec2 offset, mat4 view_proj);

#endif /* !INVENTORY_H */
