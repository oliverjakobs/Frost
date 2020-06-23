#ifndef ECS_DEBUG_RENDER_SYSTEM_H
#define ECS_DEBUG_RENDER_SYSTEM_H

#include "../Ecs.h"

void DebugRenderSystem(Ecs* ecs, ComponentTable* components, const float* mat_view_proj);

#endif /* !ECS_DEBUG_RENDER_SYSTEM_H */