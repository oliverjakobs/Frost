#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Ignis/Ignis.h"
#include "clib/dictionary.h"

typedef struct
{
	clib_hashmap textures;	/* <str,IgnisTexture> */
	clib_hashmap fonts;	/* <str,IgnisFont> */
} ResourceManager;

int ResourceManagerInit(ResourceManager* resources, const char* path);
void ResourceManagerDestroy(ResourceManager* resources);

IgnisTexture2D* ResourceManagerAddTexture2D(ResourceManager* resources, const char* name, const char* path, int rows, int columns);
IgnisFont* ResourceManagerAddFont(ResourceManager* resources, const char* name, const char* path, float size);

IgnisTexture2D* ResourceManagerGetTexture2D(ResourceManager* resources, const char* name);
IgnisFont* ResourceManagerGetFont(ResourceManager* resources, const char* name);

const char* ResourceManagerGetTexture2DName(ResourceManager* resources, IgnisTexture2D* texture);
const char* ResourceManagerGetFontName(ResourceManager* resources, IgnisFont* font);

#ifdef __cplusplus
}
#endif

#endif /* !RESOURCE_MANAGER_H */