#ifndef TILE_RENDERER_H
#define TILE_RENDERER_H

#include "tile_body.h"

#include "ignis/renderer/renderer.h"

typedef struct TileRenderer
{
    IgnisQuad quad;
    IgnisShader shader;

    size_t tile_count;

    IgnisBuffer offset_buffer;
    IgnisBuffer frames_buffer;

    vec2* offsets;
    GLuint* frames;

    GLint uniform_location_view_proj;
    GLint uniform_location_model;
    GLint uniform_location_rows;
    GLint uniform_location_cols;
} TileRenderer;

int TileRendererInit(TileRenderer* renderer);
void TileRendererDestroy(TileRenderer* renderer);

void TileRendererBindMap(TileRenderer* renderer, const TileMap* map);
void TileRendererClear(TileRenderer* renderer);

void TileRendererUpdateBuffers(TileRenderer* renderer, const TileMap* map);

void TileMapRender(TileRenderer* renderer, IgnisTexture2D* texture, mat4 view_proj, int rows, int cols);

void TileMapRenderDebug(const TileMap* map);
void TileBodyRenderDebug(const TileBody* body);

#endif // !TILE_RENDERER_H
