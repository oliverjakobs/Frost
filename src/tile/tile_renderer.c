#include "tile_renderer.h"

int TileRendererInit(TileRenderer* renderer)
{
    /* TODO error checking */
    ignisCreateQuadTextured(&renderer->quad, GL_STATIC_DRAW);

    ignisCreateShadervf(&renderer->shader, "res/shaders/tile.vert", "res/shaders/tile.frag");
    ignisUseShader(&renderer->shader);
    ignisSetUniform1i(&renderer->shader, "u_Texture", 0);

    renderer->uniform_location_view_proj = ignisGetUniformLocation(&renderer->shader, "u_ViewProjection");
    renderer->uniform_location_model = ignisGetUniformLocation(&renderer->shader, "u_Model");
    renderer->uniform_location_rows = ignisGetUniformLocation(&renderer->shader, "u_Rows");
    renderer->uniform_location_cols = ignisGetUniformLocation(&renderer->shader, "u_Columns");

    return 1;
}

void TileRendererDestroy(TileRenderer* renderer)
{
    TileRendererClear(renderer);

    ignisDeleteShader(&renderer->shader);
    ignisDeleteQuad(&renderer->quad);
}

void TileRendererBindMap(TileRenderer* renderer, const TileMap* map)
{
    renderer->tile_count = map->rows * map->cols;

    ignisBindQuad(&renderer->quad);

    ignisGenerateArrayBuffer(&renderer->offset_buffer, sizeof(vec2) * renderer->tile_count, NULL, GL_DYNAMIC_DRAW);
    ignisVertexAttribFloat(2, 2, GL_FALSE, 2, 0);
    ignisVertexAttribDivisor(2, 1);

    ignisGenerateArrayBuffer(&renderer->frames_buffer, sizeof(GLuint) * renderer->tile_count, NULL, GL_DYNAMIC_DRAW);
    ignisVertexAttribInt(3, 1, 1, 0);
    ignisVertexAttribDivisor(3, 1);

    renderer->offsets = malloc(sizeof(vec2) * renderer->tile_count);
    renderer->frames = malloc(sizeof(GLuint) * renderer->tile_count);
    TileRendererUpdateBuffers(renderer, map);

    ignisUseShader(&renderer->shader);
    ignisSetUniform1f(&renderer->shader, "u_TileSize", map->tile_size);
}

void TileRendererClear(TileRenderer* renderer)
{
    if (renderer->tile_count == 0) return;

    renderer->tile_count = 0;

    ignisDeleteBuffer(&renderer->offset_buffer);
    ignisDeleteBuffer(&renderer->frames_buffer);

    free(renderer->offsets);
    free(renderer->frames);
}

void TileRendererUpdateBuffers(TileRenderer* renderer, const TileMap* map)
{
    for (size_t i = 0; i < renderer->tile_count; ++i)
    {
        renderer->offsets[i].x = map->tiles[i].pos.x;
        renderer->offsets[i].y = map->tiles[i].pos.y;

        renderer->frames[i] = map->tiles[i].id;
    }

    ignisBufferSubData(&renderer->offset_buffer, 0, sizeof(vec2) * renderer->tile_count, renderer->offsets);
    ignisBufferSubData(&renderer->frames_buffer, 0, sizeof(GLuint) * renderer->tile_count, renderer->frames);
}

void TileMapRender(TileRenderer* renderer, IgnisTexture2D* texture, mat4 view_proj)
{
    if (renderer->tile_count == 0) return;

    mat4 model = mat4_indentity();

    ignisUseShader(&renderer->shader);
    ignisSetUniformMat4l(&renderer->shader, renderer->uniform_location_view_proj, view_proj.v);
    ignisSetUniformMat4l(&renderer->shader, renderer->uniform_location_model, model.v);

    ignisSetUniform1il(&renderer->shader, renderer->uniform_location_rows, texture->rows);
    ignisSetUniform1il(&renderer->shader, renderer->uniform_location_cols, texture->cols);

    ignisBindTexture2D(texture, 0);
    ignisDrawQuadElementsInstanced(&renderer->quad, GL_TRIANGLES, (GLsizei)renderer->tile_count);
}

void TileMapRenderDebug(const TileMap* map)
{
    float tile_size = map->tile_size;

    /* map border */
    Primitives2DRenderRect(0.0f, 0.0f, map->cols * tile_size, map->rows * tile_size, IGNIS_WHITE);

    if (TileMapGetBorder(map, TILE_BOTTOM))
    {
        for (size_t i = 0; i < map->cols; ++i)
            Primitives2DRenderRect(i * tile_size, -tile_size, tile_size, tile_size, IGNIS_BLACK);
    }

    if (TileMapGetBorder(map, TILE_TOP))
    {
        float y = map->rows * tile_size;
        for (size_t i = 0; i < map->cols; ++i)
            Primitives2DRenderRect(i * tile_size, y, tile_size, tile_size, IGNIS_BLACK);
    }

    if (TileMapGetBorder(map, TILE_LEFT))
    {
        for (size_t i = 0; i < map->rows; ++i)
            Primitives2DRenderRect(-tile_size, i * tile_size, tile_size, tile_size, IGNIS_BLACK);
    }

    if (TileMapGetBorder(map, TILE_RIGHT))
    {
        float x = map->cols * tile_size;
        for (size_t i = 0; i < map->rows; ++i)
            Primitives2DRenderRect(x, i * tile_size, tile_size, tile_size, IGNIS_BLACK);
    }

    for (size_t i = 0; i < (map->rows * map->cols); ++i)
    {
        Tile* tile = &map->tiles[i];
        switch (tile->type)
        {
        case TILE_SOLID:
            Primitives2DRenderRect(tile->pos.x, tile->pos.y, tile_size, tile_size, IGNIS_WHITE);
            break;
        case TILE_SLOPE_RIGHT:
        {
            float vertices[] =
            {
                tile->pos.x, tile->pos.y,
                tile->pos.x + tile_size, tile->pos.y,
                tile->pos.x + tile_size, tile->pos.y + tile_size
            };
            Primitives2DRenderPoly(vertices, 6, 0.0f, 0.0f, IGNIS_BLUE);
            break;
        }
        case TILE_SLOPE_LEFT:
        {
            float vertices[] =
            {
                tile->pos.x, tile->pos.y,
                tile->pos.x + tile_size, tile->pos.y,
                tile->pos.x, tile->pos.y + tile_size
            };
            Primitives2DRenderPoly(vertices, 6, 0.0f, 0.0f, IGNIS_BLUE);
            break;
        }
        case TILE_PLATFORM:
        {
            float height = 4.0f;
            float x = tile->pos.x;
            float y = tile->pos.y + tile_size - height;
            Primitives2DRenderRect(x, y, tile_size, height, IGNIS_CYAN);
            break;
        }
        default:
            break;
        }
        
    }
}

void TileBodyRenderDebug(const TileBody* body)
{
    vec2 pos = vec2_sub(body->position, body->half_dim);
    vec2 dim = vec2_mult(body->half_dim, 2.0f);

    if (body->type == TILE_BODY_DYNAMIC)
    {
        Primitives2DRenderRect(pos.x, pos.y, dim.x, dim.y, body->on_slope ? IGNIS_BLUE : IGNIS_GREEN);

        line sensor = TileBodyGetSensor(body, TILE_BOTTOM, body->position);
        Primitives2DRenderLine(sensor.start.x, sensor.start.y, sensor.end.x, sensor.end.y, IGNIS_WHITE);

        sensor = TileBodyGetSensor(body, TILE_TOP, body->position);
        Primitives2DRenderLine(sensor.start.x, sensor.start.y, sensor.end.x, sensor.end.y, IGNIS_WHITE);

        sensor = TileBodyGetSensor(body, TILE_LEFT, body->position);
        Primitives2DRenderLine(sensor.start.x, sensor.start.y, sensor.end.x, sensor.end.y, IGNIS_WHITE);

        sensor = TileBodyGetSensor(body, TILE_RIGHT, body->position);
        Primitives2DRenderLine(sensor.start.x, sensor.start.y, sensor.end.x, sensor.end.y, IGNIS_WHITE);
    }
    else
    {
        Primitives2DRenderRect(pos.x, pos.y, dim.x, dim.y, IGNIS_WHITE);
    }
}
