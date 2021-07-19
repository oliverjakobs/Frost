#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"

typedef enum
{
    INV_ALIGN_H_NONE,
    INV_ALIGN_H_LEFT,
    INV_ALIGN_H_CENTER,
    INV_ALIGN_H_RIGHT
} InventoryAlignH;

typedef enum
{
    INV_ALIGN_V_NONE,
    INV_ALIGN_V_TOP,
    INV_ALIGN_V_CENTER,
    INV_ALIGN_V_BOTTOM
} InventoryAlignV;

typedef enum
{
    INVENTORY_CLOSED,
    INVENTORY_OPEN,
    INVENTORY_FIXED
} InventoryState;

typedef struct
{
    vec2 pos;

    float padding; /* padding at border */
    float spacing; /* spacing between cells */
    float cell_size;

    int rows, cols;

    InventoryState state;

    ItemID* cells;
} Inventory;

void InventoryLoad(char* ini, Ecs* ecs, EcsEntityID entity, vec2 screen_size);
void InventoryRelease(Inventory* inv);

void InventoryToggle(Inventory* inv);

int InventoryClampOffset(const Inventory* inv, float pos);
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