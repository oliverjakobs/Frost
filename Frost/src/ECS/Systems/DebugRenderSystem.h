#ifndef ECS_DEBUG_RENDER_SYSTEM_H
#define ECS_DEBUG_RENDER_SYSTEM_H

#include "../Entity.h"

void EcsSystemDebugRenderPre(const float* mat_view_proj);
void EcsSystemDebugRender(EcsEntity* entity, ComponentTable* components);
void EcsSystemDebugRenderPost();

#endif /* !ECS_DEBUG_RENDER_SYSTEM_H */