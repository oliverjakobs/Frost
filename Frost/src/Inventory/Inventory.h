#ifndef INVENTORY_H
#define INVENTORY_H

#include "Camera/Camera.h"
#include "Graphics/Renderer.h"

#define NULL_ITEM (-1)

typedef enum
{
	INV_HALIGN_LEFT,
	INV_HALIGN_CENTER,
	INV_HALIGN_RIGHT
} InventoryHorizontalAlign;

typedef enum
{
	INV_VALIGN_TOP,
	INV_VALIGN_CENTER,
	INV_VALIGN_BOTTOM
} InventoryVerticalAlign;

typedef struct
{
	vec2 screen_size;

	float cell_size;
	float padding;

	IgnisTexture2D* item_atlas;
} InventoryTheme;

void InventoryThemeLoad(InventoryTheme* theme, IgnisTexture2D* item_atlas, vec2 screen_size, float cell_size, float padding);

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

int InventoryInit(Inventory* inv, vec2 pos, int rows, int columns, InventoryTheme* theme);
int InventoryInitAligned(Inventory* inv, InventoryHorizontalAlign h_align, InventoryVerticalAlign v_align, int rows, int columns, InventoryTheme* theme);
void InventoryFree(Inventory* inv);

int InventoryGetCellIndex(Inventory* inv, int row, int column);

void InventorySetCellContent(Inventory* inv, int index, int itemID);
int InventoryGetCellContent(Inventory* inv, int index);

void InventoryMoveCellContent(Inventory* dst_inv, int dst_cell, Inventory* src_inv, int src_cell);

/* system */
void InventoryUpdateSystem(Inventory* invs, size_t count, InventoryTheme* theme, float deltatime);
void InventoryRenderSystem(Inventory* invs, size_t count, InventoryTheme* theme, Camera* camera);

#endif /* !INVENTORY_H */
