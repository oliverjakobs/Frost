#ifndef ECS_TEMPLATE_LOADER_H
#define ECS_TEMPLATE_LOADER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ECS/Entity.h"
#include "Graphics/ResourceManager.h"

EcsEntity* EcsEntityLoadTemplate(const char* path, ResourceManager* res);

#ifdef __cplusplus
}
#endif

#endif /* !ECS_TEMPLATE_LOADER_H */