#ifndef INVENTORY_H
#define INVENTORY_H

#include "Scenes/Scene.h"

#define NULL_ITEM (-1)

typedef struct
{
	int itemID;
	vec2 pos;
} InventoryCell;

typedef enum
{
	INVENTORY_CLOSED,
	INVENTORY_OPEN,
	INVENTORY_FIXED
} InventoryState;

typedef enum
{
	INV_HALIGN_LEFT,
	INV_HALIGN_CENTER,
	INV_HALIGN_RIGHT
} InventoryHAlign;

typedef enum
{
	INV_VALIGN_TOP,
	INV_VALIGN_CENTER,
	INV_VALIGN_BOTTOM
} InventoryVAlign;

typedef struct
{
	vec2 pos;
	vec2 size;

	int rows;
	int columns;

	InventoryState state;

	InventoryCell* cells;
} Inventory;

void InventoryLoad(Scene* scene, EcsEntityID entity, char* json);

void InventoryFree(Inventory* inv);

void InventoryToggle(Inventory* inv);

int InventoryGetCellIndex(Inventory* inv, int row, int column);

void InventorySetCellContent(Inventory* inv, int index, int itemID);
int InventoryGetCellContent(Inventory* inv, int index);

void InventoryMoveCellContent(Inventory* dst_inv, int dst_cell, Inventory* src_inv, int src_cell);

InventoryState InventoryParseState(const char* str, size_t max_count);
InventoryHAlign InventoryParseHAlign(const char* str, size_t max_count);
InventoryVAlign InventoryParseVAlign(const char* str, size_t max_count);

#endif /* !INVENTORY_H */