#ifndef TILE_RENDERER_H
#define TILE_RENDERER_H

#include "tile.h"

#include "Graphics/Renderer.h"

typedef struct TileRenderer
{
    IgnisQuad quad;
    IgnisShader shader;

    size_t tile_count;
    vec2* offsets;
    GLuint* frames;

    GLint uniform_location_view_proj;
    GLint uniform_location_model;
    GLint uniform_location_rows;
    GLint uniform_location_cols;
} TileRenderer;

int TileRendererInit(TileRenderer* renderer, const TileMap* map);
void TileRendererDestroy(TileRenderer* renderer);

void TileRendererUpdateBuffers(TileRenderer* renderer, const TileMap* map);

void TileMapRender(TileRenderer* renderer, IgnisTexture2D* texture, mat4 view_proj);

void TileMapRenderDebug(const TileMap* map, mat4 view_proj);
void TileBodyRenderDebug(const TileBody* body);

#endif // !TILE_RENDERER_H
