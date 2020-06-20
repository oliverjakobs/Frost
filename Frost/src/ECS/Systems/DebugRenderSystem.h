#ifndef ECS_DEBUG_RENDER_SYSTEM_H
#define ECS_DEBUG_RENDER_SYSTEM_H

#include "../Entity.h"

void EcsSystemDebugRenderPre(const float* mat_view_proj);
void EcsSystemDebugRender(ComponentTable* components, const char* entity);
void EcsSystemDebugRenderPost();

#endif /* !ECS_DEBUG_RENDER_SYSTEM_H */