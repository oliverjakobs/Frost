#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "Scenes/Scene.h"

void RenderSystem(const Ecs* ecs, const Scene* scene, const float* mat_view_proj);
void DebugRenderSystem(const Ecs* ecs, const Scene* scene, const float* mat_view_proj);

#endif /* !RENDER_SYSTEM_H */
