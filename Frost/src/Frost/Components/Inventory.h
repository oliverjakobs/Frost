#ifndef INVENTORY_H
#define INVENTORY_H

#include "Scenes/Scene.h"

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

	InventoryCell* cells;
} Inventory;


void InventoryLoad(Scene* scene, EcsEntityID entity, vec2 pos, int z_index, char* json);

void InventoryFree(Inventory* inv);

int InventoryGetCellIndex(Inventory* inv, int row, int column);

void InventorySetCellContent(Inventory* inv, int index, int itemID);
int InventoryGetCellContent(Inventory* inv, int index);

void InventoryMoveCellContent(Inventory* dst_inv, int dst_cell, Inventory* src_inv, int src_cell);

#endif /* !INVENTORY_H */