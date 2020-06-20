#ifndef ECS_RENDER_SYSTEM_H
#define ECS_RENDER_SYSTEM_H

#include "../Entity.h"

void EcsSystemRender(ComponentTable* components, const float* mat_view_proj);

#endif /* !ECS_RENDER_SYSTEM_H */
