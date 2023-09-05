#ifndef SCENE_RESOURCES_H
#define SCENE_RESOURCES_H

#include "ignis/renderer/renderer.h"

#include "toolbox/tb_hashmap.h"

typedef struct
{
    tb_hashmap textures;	/* <str,IgnisTexture> */
} Resources;

int ResourcesInit(Resources* res, void* allocator);
void ResourcesDestroy(Resources* res);

void ResourcesClear(Resources* res);

IgnisTexture2D* ResourcesLoadTexture2D(Resources* res, const char* path);
const char* ResourcesGetTexture2DPath(const Resources* res, const IgnisTexture2D* texture);

#endif /* !SCENE_RESOURCES_H */
