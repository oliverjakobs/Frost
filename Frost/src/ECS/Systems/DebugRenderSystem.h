#ifndef ECS_DEBUG_RENDER_SYSTEM_H
#define ECS_DEBUG_RENDER_SYSTEM_H

#include "../Entity.h"

void EcsSystemDebugRender(ComponentTable* components, const float* mat_view_proj);

#endif /* !ECS_DEBUG_RENDER_SYSTEM_H */