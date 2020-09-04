#ifndef INVENTORY_H
#define INVENTORY_H

#include "Camera/Camera.h"
#include "Graphics/Renderer.h"

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
} Inventory;

int InventoryInit(Inventory* inv, vec2 pos, int rows, int columns, float cell_size, float padding);
void InventoryFree(Inventory* inv);

int InventoryGetCellIndex(Inventory* inv, int row, int column);
int InventoryGetCellAt(Inventory* inv, vec2 pos);

void InventorySetCellContent(Inventory* inv, int index, int itemID);
int InventoryGetCellContent(Inventory* inv, int index);

void InventoryMoveCellContent(Inventory* dst_inv, int dst_cell, Inventory* src_inv, int src_cell);

void InventoryUpdateSystem(Inventory* invs, size_t count, Camera* camera, float deltatime);
void InventoryRenderSystem(Inventory* invs, size_t count, IgnisTexture2D* item_atlas, Camera* camera);

#endif /* !INVENTORY_H */
