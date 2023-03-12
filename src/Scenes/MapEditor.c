#include "MapEditor.h"

#include "SceneEditor.h"

#include "Frost/Frost.h"

#define PALETTE_TILE_SIZE   32.0f
#define PALETTE_PADDING     20.0f
#define PALETTE_HEIGHT      ((2 * PALETTE_PADDING) + PALETTE_TILE_SIZE)

void MapEditorReset(MapEditor* editor)
{
    editor->tile_hover = NULL;
    editor->palette_hover = -1;
    editor->palette_select = 0;
    editor->set_mode = TILE_REPLACE;
}

static int MapEditorPaletteGetHover(vec2 m, size_t palette_size)
{
    m.x -= PALETTE_PADDING;
    m.y -= PALETTE_PADDING;

    if (m.x >= 0.0f && m.x <= (palette_size * PALETTE_TILE_SIZE) && m.y >= 0.0f && m.y <= PALETTE_TILE_SIZE)
        return (int)floorf(m.x / PALETTE_TILE_SIZE);

    return -1;
}

/* TODO insertion mode: REPLACE, INSERT(if not empty), DELETE, DELETEMATCH(if tile matches selected delete it) */
void MapEditorOnEvent(MapEditor* editor, Scene* scene, const MinimalEvent* e)
{
    vec2 mouse = { 0 };
    if (MinimalEventMouseMoved(e, &mouse.x, &mouse.y))
    {
        mouse = CameraGetMousePos(&scene->camera, mouse);
        if (mouse.y > PALETTE_HEIGHT)
        {
            editor->tile_hover = TileMapAtPos(&scene->map, vec2_add(mouse, scene->camera.position));
            editor->palette_hover = -1;
        }
        else
        {
            editor->tile_hover = NULL;
            editor->palette_hover = MapEditorPaletteGetHover(mouse, scene->map.types_count);
        }
    }

    if (MinimalEventMouseButtonPressed(e, &mouse.x, &mouse.y) == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (editor->tile_hover)
        {
            mouse = CameraGetMousePosView(&scene->camera, mouse);
            TileMapSetAtPos(&scene->map, mouse, editor->palette_select, editor->set_mode);
            TileRendererUpdateBuffers(&scene->renderer, &scene->map);
        }

        if (editor->palette_hover >= 0)
            editor->palette_select = editor->palette_hover;
    }
}

void MapEditorOnUpdate(MapEditor* editor, Scene* scene, float deltatime)
{
    
}

void MapEditorOnRender(const MapEditor* editor, Scene* scene, int show_grid, float padding)
{
    Primitives2DSetViewProjection(CameraGetViewProjectionPtr(&scene->camera));

    if (show_grid) SceneEditorRenderGrid(scene, padding);
    TileMapRenderDebug(&scene->map);

    if (editor->tile_hover)
    {
        vec2 pos = editor->tile_hover->pos;
        float tile_size = scene->map.tile_size;

        Primitives2DRenderRect(pos.x,pos.y, tile_size, tile_size, IGNIS_GREEN);
    }

    Primitives2DFlush();

    /* UI */
    Primitives2DSetViewProjection(CameraGetProjectionPtr(&scene->camera));

    Primitives2DFillRect(0.0f, 0.0f, scene->camera.size.x, PALETTE_HEIGHT, IGNIS_LIGHT_GREY);

    Primitives2DFlush();

    /* Palette */
    Batch2DSetViewProjection(CameraGetProjectionPtr(&scene->camera));

    for (size_t i = 0; i < scene->map.types_count; ++i)
    {
        float x = PALETTE_PADDING + (i * PALETTE_TILE_SIZE);
        float y = PALETTE_PADDING;
        float w = PALETTE_TILE_SIZE;
        float h = PALETTE_TILE_SIZE;

        float src_x, src_y, src_w, src_h;
        GetTexture2DSrcRect(scene->tile_set, i, &src_x, &src_y, &src_w, &src_h);

        Batch2DRenderTextureSrc(scene->tile_set, x, y, w, h, src_x, src_y, src_w, src_h);
    }

    Batch2DFlush();

    /* UI Overlay */
    Primitives2DSetViewProjection(CameraGetProjectionPtr(&scene->camera));

    if (editor->palette_hover >= 0)
    {
        float x = PALETTE_PADDING + (editor->palette_hover * PALETTE_TILE_SIZE);
        float y = PALETTE_PADDING;

        IgnisColorRGBA color = IGNIS_WHITE;
        ignisBlendColorRGBA(&color, 0.2f);

        Primitives2DFillRect(x, y, PALETTE_TILE_SIZE, PALETTE_TILE_SIZE, color);
    }

    if (editor->palette_select >= 0)
    {
        float x = PALETTE_PADDING + (editor->palette_select * PALETTE_TILE_SIZE);
        float y = PALETTE_PADDING;

        Primitives2DRenderRect(x, y, PALETTE_TILE_SIZE, PALETTE_TILE_SIZE, IGNIS_WHITE);
    }

    Primitives2DFlush();
}
