#ifndef INVENTORY_H
#define INVENTORY_H

#include "Camera/Camera.h"

#define NULL_ITEM (-1)

typedef struct
{
	int itemID;
	vec2 pos;
} InventoryCell;

typedef struct
{
	vec2 pos;
	vec2 size;

	int rows;
	int columns;

	float cell_size;
	float padding;

	InventoryCell* cells;

	int hover;
} Inventory;

int InventoryInit(Inventory* inv, vec2 pos, int rows, int columns, float cell_size, float padding);
void InventoryFree(Inventory* inv);

int InventoryGetCellIndex(Inventory* inv, int row, int column);

int InventoryGetCellAt(Inventory* inv, vec2 pos);

void InventorySetCellContent(Inventory* inv, int index, int itemID);
int InventoryGetCellContent(Inventory* inv, int index);

void InventoryUpdate(Inventory* inv, Camera* camera, float deltatime);
void InventoryRender(Inventory* inv, vec2 offset, mat4 view_proj);

#endif /* !INVENTORY_H */
