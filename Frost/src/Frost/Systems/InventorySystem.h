#ifndef INVENTORY_SYSTEM_H
#define INVENTORY_SYSTEM_H

#include "Ecs/Ecs.h"

#include "Graphics/Resources.h"
#include "../Components/Inventory.h"

void InventorySystemLoad(IgnisTexture2D* item_atlas, vec2 screen_size, float cell_size, float padding);

typedef enum
{
	INV_HALIGN_LEFT,
	INV_HALIGN_CENTER,
	INV_HALIGN_RIGHT
} InventoryHAlign;

InventoryHAlign InventorySystemGetHAlign(const char* str);

typedef enum
{
	INV_VALIGN_TOP,
	INV_VALIGN_CENTER,
	INV_VALIGN_BOTTOM
} InventoryVAlign;

InventoryVAlign InventorySystemGetVAlign(const char* str);

int InventoryInit(Inventory* inv, vec2 pos, int rows, int columns);
int InventoryInitAligned(Inventory* inv, InventoryHAlign h_align, InventoryVAlign v_align, int rows, int columns);

void InventoryUpdateSystem(Ecs* ecs, float deltatime);
void InventoryRenderSystem(Ecs* ecs, const float* mat_view_proj);

#endif /* !INVENTORY_SYSTEM_H */