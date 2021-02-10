#ifndef INVENTORY_SYSTEM_H
#define INVENTORY_SYSTEM_H

#include "Scenes/Scene.h"

void InventoryUpdateSystem(Ecs* ecs, Scene* scene, float deltatime);
void InventoryRenderSystem(const Ecs* ecs, const Scene* scene, const float* mat_view_proj);

#endif /* !INVENTORY_SYSTEM_H */