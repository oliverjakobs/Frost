#ifndef INVENTORY_SYSTEM_H
#define INVENTORY_SYSTEM_H

#include "Ecs/Ecs.h"

#include "Camera/Camera.h"
#include "Graphics/Resources.h"

int InventorySystemInit(IgnisTexture2D* item_atlas, Camera* camera);

void InventoryUpdateSystem(Ecs* ecs, float deltatime);
void InventoryRenderSystem(const Ecs* ecs, const float* mat_view_proj);

#endif /* !INVENTORY_SYSTEM_H */