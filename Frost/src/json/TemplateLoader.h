#ifndef TEMPLATE_LOADER_H
#define TEMPLATE_LOADER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "SceneManager.h"

EcsEntity* TemplateLoadEntity(const char* path, ResourceManager* res);

#ifdef __cplusplus
}
#endif

#endif /* !TEMPLATE_LOADER_H */