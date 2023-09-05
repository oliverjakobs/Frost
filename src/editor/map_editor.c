#include "map_editor.h"

#include "scene_editor.h"

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
    if (minimalEventMouseMoved(e, &mouse.x, &mouse.y))
    {
        mouse = cameraGetMousePos(&scene->camera, mouse);
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

    if (minimalEventMouseButtonPressed(e, &mouse.x, &mouse.y) == MINIMAL_MOUSE_BUTTON_LEFT)
    {
        if (editor->tile_hover)
        {
            mouse = cameraGetMousePosView(&scene->camera, mouse);
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
    ignisPrimitivesRendererSetViewProjection(cameraGetViewProjectionPtr(&scene->camera));

    if (show_grid) SceneEditorRenderGrid(scene, padding);
    TileMapRenderDebug(&scene->map);

    if (editor->tile_hover)
    {
        vec2 pos = editor->tile_hover->pos;
        float tile_size = scene->map.tile_size;

        ignisPrimitives2DRenderRect(pos.x,pos.y, tile_size, tile_size, IGNIS_GREEN);
    }

    ignisPrimitivesRendererFlush();

    /* UI */
    ignisPrimitivesRendererSetViewProjection(cameraGetProjectionPtr(&scene->camera));

    ignisPrimitives2DFillRect(0.0f, 0.0f, scene->camera.size.x, PALETTE_HEIGHT, IGNIS_LIGHT_GREY);

    ignisPrimitivesRendererFlush();

    /* Palette */
    ignisBatch2DSetViewProjection(cameraGetProjectionPtr(&scene->camera));

    for (uint32_t i = 0; i < scene->map.types_count; ++i)
    {
        IgnisRect rect = {
            PALETTE_PADDING + (i * PALETTE_TILE_SIZE),
            PALETTE_PADDING,
            PALETTE_TILE_SIZE,
            PALETTE_TILE_SIZE
        };

        IgnisRect src = ignisGetTexture2DSrcRect(scene->tile_set, scene->tile_set_size.x, scene->tile_set_size.y, i);
        ignisBatch2DRenderTextureSrc(scene->tile_set, rect, src);
    }

    ignisBatch2DFlush();

    /* UI Overlay */
    ignisPrimitivesRendererSetViewProjection(cameraGetProjectionPtr(&scene->camera));

    if (editor->palette_hover >= 0)
    {
        float x = PALETTE_PADDING + (editor->palette_hover * PALETTE_TILE_SIZE);
        float y = PALETTE_PADDING;

        IgnisColorRGBA color = IGNIS_WHITE;
        ignisBlendColorRGBA(&color, 0.2f);

        ignisPrimitives2DFillRect(x, y, PALETTE_TILE_SIZE, PALETTE_TILE_SIZE, color);
    }

    if (editor->palette_select >= 0)
    {
        float x = PALETTE_PADDING + (editor->palette_select * PALETTE_TILE_SIZE);
        float y = PALETTE_PADDING;

        ignisPrimitives2DRenderRect(x, y, PALETTE_TILE_SIZE, PALETTE_TILE_SIZE, IGNIS_WHITE);
    }

    ignisPrimitivesRendererFlush();
}
