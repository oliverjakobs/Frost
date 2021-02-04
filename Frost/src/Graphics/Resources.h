#ifndef RESOURCES_H
#define RESOURCES_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Ignis/Ignis.h"
#include "toolbox/tb_hashmap.h"
#include "memory/arena.h"

typedef struct
{
	tb_hashmap textures;	/* <str,IgnisTexture> */
	tb_hashmap fonts;	/* <str,IgnisFont> */
	Arena arena;
} Resources;

int ResourcesInit(Resources* resources, const char* path);
void ResourcesDestroy(Resources* resources);

IgnisTexture2D* ResourcesAddTexture2D(Resources* resources, const char* name, const char* path, int rows, int columns);
IgnisFont* ResourcesAddFont(Resources* resources, const char* name, const char* path, float size);

IgnisTexture2D* ResourcesGetTexture2D(const Resources* resources, const char* name);
IgnisFont* ResourcesGetFont(const Resources* resources, const char* name);

const char* ResourcesGetTexture2DName(const Resources* resources, const IgnisTexture2D* texture);
const char* ResourcesGetFontName(const Resources* resources, const IgnisFont* font);

#ifdef __cplusplus
}
#endif

#endif /* !RESOURCES_H */