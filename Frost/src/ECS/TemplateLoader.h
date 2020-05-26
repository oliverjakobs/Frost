#ifndef ECS_TEMPLATE_LOADER_H
#define ECS_TEMPLATE_LOADER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ECS/Entity.h"
#include "Scene/SceneManager.h"

EcsEntity* EcsEntityLoadTemplate(SceneManager* manager, const char* templ);

#ifdef __cplusplus
}
#endif

#endif /* !ECS_TEMPLATE_LOADER_H */