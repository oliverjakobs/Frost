#ifndef INVENTORY_SYSTEM_H
#define INVENTORY_SYSTEM_H

#include "Ecs/Ecs.h"

#include "Graphics/Resources.h"
#include "../Components/Inventory.h"

int InventorySystemInit(IgnisTexture2D* item_atlas, Camera* camera, float cell_size, float padding);

int InventoryCreate(Inventory* inv, vec2 pos, int rows, int columns);
int InventoryCreateAligned(Inventory* inv, InventoryHAlign h_align, InventoryVAlign v_align, int rows, int columns);

void InventoryUpdateSystem(Ecs* ecs, float deltatime);
void InventoryRenderSystem(const Ecs* ecs, const float* mat_view_proj);

#endif /* !INVENTORY_SYSTEM_H */