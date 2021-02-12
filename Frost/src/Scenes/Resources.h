#ifndef SCENE_RESOURCES_H
#define SCENE_RESOURCES_H

#include "Graphics/Renderer.h"

#include "toolbox/tb_hashmap.h"

typedef struct
{
    tb_hashmap textures;	/* <str,IgnisTexture> */
} Resources;

int ResourcesInit(Resources* res);
void ResourcesDestroy(Resources* res);

void ResourcesClear(Resources* res);

IgnisTexture2D* ResourcesAddTexture2D(Resources* res, const char* name, const char* path, int rows, int cols);
IgnisTexture2D* ResourcesGetTexture2D(const Resources* res, const char* name);
const char* ResourcesGetTexture2DName(const Resources* res, const IgnisTexture2D* texture);

#endif /* !SCENE_RESOURCES_H */
