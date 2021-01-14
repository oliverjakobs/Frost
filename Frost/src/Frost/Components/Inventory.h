#ifndef INVENTORY_H
#define INVENTORY_H

#include "Scenes/Scene.h"

#define NULL_ITEM (-1)
typedef int32_t ItemID;

typedef enum
{
	INV_HALIGN_NONE,
	INV_HALIGN_LEFT,
	INV_HALIGN_CENTER,
	INV_HALIGN_RIGHT
} InvHAlign;

typedef enum
{
	INV_VALIGN_NONE,
	INV_VALIGN_TOP,
	INV_VALIGN_CENTER,
	INV_VALIGN_BOTTOM
} InvVAlign;

typedef enum
{
	INVENTORY_CLOSED,
	INVENTORY_OPEN,
	INVENTORY_FIXED
} InventoryState;

/* TODO implement spacing */
typedef struct
{
	vec2 pos;

	float padding;
	float spacing;
	float cell_size;

	int rows;
	int cols;

	InventoryState state;

	ItemID* cells;
} Inventory;

void InventoryLoad(Scene* scene, EcsEntityID entity, char* json);

int InventoryCreate(Inventory* inv, InventoryState state, vec2 pos, int rows, int cols);
void InventorySetLayout(Inventory* inv, float cell_size, float padding, float spacing);
void InventoryAlign(Inventory* inv, InvHAlign h_align, InvVAlign v_align, vec2 screen_size);
void InventoryFree(Inventory* inv);

void InventoryToggle(Inventory* inv);

int InventoryGetCellIndex(const Inventory* inv, int row, int column);
int InventoryGetCellAt(const Inventory* inv, vec2 pos);

vec2 InventoryGetCellPos(const Inventory* inv, int index);
float InventoryGetCellOffset(const Inventory* inv, int index);

void InventorySetCellContent(Inventory* inv, int index, ItemID itemID);
ItemID InventoryGetCellContent(const Inventory* inv, int index);

void InventoryMoveCellContent(Inventory* dst_inv, int dst_cell, Inventory* src_inv, int src_cell);

float InventoryGetWidth(const Inventory* inv);
float InventoryGetHeight(const Inventory* inv);

#endif /* !INVENTORY_H */