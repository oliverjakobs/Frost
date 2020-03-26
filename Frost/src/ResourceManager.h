#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Ignis/Ignis.h"
#include "clib/hashmap.h"

typedef struct
{
	struct hashmap textures;
	struct hashmap fonts;
} ResourceManager;

int ResourceManagerInit(ResourceManager* manager, const char* path);
void ResourceManagerDestroy(ResourceManager* manager);

void ResourceManagerAddTexture(ResourceManager* manager, const char* name, const char* path, int rows, int columns);
IgnisTexture* ResourceManagerGetTexture(ResourceManager* manager, const char* name);

void ResourceManagerAddFont(ResourceManager* manager, const char* name, const char* path, float size);
IgnisFont* ResourceManagerGetFont(ResourceManager* manager, const char* name);

#ifdef __cplusplus
}
#endif

#endif /* !RESOURCE_MANAGER_H */