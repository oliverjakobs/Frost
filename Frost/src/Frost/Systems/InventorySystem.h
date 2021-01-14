#ifndef INVENTORY_SYSTEM_H
#define INVENTORY_SYSTEM_H

#include "Scenes/Scene.h"

#include "Camera/Camera.h"
#include "Graphics/Resources.h"

int InventorySystemInit(IgnisTexture2D* item_atlas);

void InventoryUpdateSystem(Ecs* ecs, const Scene* scene, float deltatime);
void InventoryRenderSystem(const Ecs* ecs, const Scene* scene, const float* mat_view_proj);

#endif /* !INVENTORY_SYSTEM_H */