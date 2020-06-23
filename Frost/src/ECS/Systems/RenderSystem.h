#ifndef ECS_RENDER_SYSTEM_H
#define ECS_RENDER_SYSTEM_H

#include "../Ecs.h"

void RenderSystem(Ecs* ecs, ComponentTable* components, const float* mat_view_proj);

#endif /* !ECS_RENDER_SYSTEM_H */
