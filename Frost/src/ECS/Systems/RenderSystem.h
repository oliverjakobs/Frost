#ifndef ECS_RENDER_SYSTEM_H
#define ECS_RENDER_SYSTEM_H

#include "../Entity.h"

void EcsSystemRenderPre(const float* mat_view_proj);
void EcsSystemRender(EcsEntity* entity, ComponentTable* components);
void EcsSystemRenderPost();

#endif /* !ECS_RENDER_SYSTEM_H */
