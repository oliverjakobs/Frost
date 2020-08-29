#ifndef RESOURCES_H
#define RESOURCES_H

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
} Resources;

int ResourcesInit(Resources* resources, const char* path);
void ResourcesDestroy(Resources* resources);

IgnisTexture2D* ResourcesAddTexture2D(Resources* resources, const char* name, const char* path, int rows, int columns);
IgnisFont* ResourcesAddFont(Resources* resources, const char* name, const char* path, float size);

IgnisTexture2D* ResourcesGetTexture2D(Resources* resources, const char* name);
IgnisFont* ResourcesGetFont(Resources* resources, const char* name);

const char* ResourcesGetTexture2DName(Resources* resources, IgnisTexture2D* texture);
const char* ResourcesGetFontName(Resources* resources, IgnisFont* font);

#ifdef __cplusplus
}
#endif

#endif /* !RESOURCES_H */