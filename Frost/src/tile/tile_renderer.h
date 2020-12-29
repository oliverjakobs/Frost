#ifndef TILE_RENDERER_H
#define TILE_RENDERER_H

#include "tilemap.h"

#include "Ignis/Ignis.h"
#include "Ignis/Quad.h"

typedef struct TileRenderer
{
    IgnisQuad quad;
    IgnisShader shader;

    size_t tile_count;

    GLint uniform_location_view_proj;
    GLint uniform_location_model;
    GLint uniform_location_rows;
    GLint uniform_location_cols;
} TileRenderer;

int TileRendererInit(TileRenderer* renderer, TileMap* map);
void TileRendererDestroy(TileRenderer* renderer);

void TileMapRender(TileRenderer* renderer, IgnisTexture2D* texture, vec2 offset, mat4 view_proj);

#endif // !TILE_RENDERER_H
