#ifndef TILE_RENDERER_H
#define TILE_RENDERER_H

#include "tilemap.h"

#include "Ignis/Ignis.h"

typedef struct TileRenderer
{
    IgnisVertexArray vao;

    IgnisShader shader;

    size_t tile_count;
} TileRenderer;

int TileRendererInit(TileRenderer* renderer, TileMap* map);
void TileRendererDestroy(TileRenderer* renderer);

void TileMapRender(TileRenderer* renderer, IgnisTexture2D* texture, vec2 offset, mat4 view_proj);

#endif // !TILE_RENDERER_H
