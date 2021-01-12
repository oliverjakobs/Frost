#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "Ecs/Ecs.h"

void RenderSystem(const Ecs* ecs, const float* mat_view_proj);
void DebugRenderSystem(const Ecs* ecs, const float* mat_view_proj);

#endif /* !RENDER_SYSTEM_H */
