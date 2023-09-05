#include "scene_editor.h"

#include "Frost/Frost.h"
#include "math/grid.h"

void WorldEditorReset(WorldEditor* editor)
{
    editor->clicked = 0;
    editor->offset = vec2_zero();
    editor->hover = ECS_NULL_ENTITY;
}

void WorldEditorOnEvent(WorldEditor* editor, Scene* scene, const MinimalEvent* e)
{
    vec2 mouse = { 0 };
    if (minimalEventMouseButtonPressed(e, &mouse.x, &mouse.y) == MINIMAL_MOUSE_BUTTON_LEFT)
    {
        if (editor->hover != ECS_NULL_ENTITY)
        {
            mouse = cameraGetMousePosView(&scene->camera, mouse);
            editor->offset = vec2_sub(mouse, GetEntityPosition(&scene->ecs, editor->hover));
            editor->clicked = 1;
        }
    }

    if (minimalEventMouseButtonReleased(e, NULL, NULL) == MINIMAL_MOUSE_BUTTON_LEFT)
    {
        editor->offset = vec2_zero();
        editor->clicked = 0;
    }

    if (minimalEventMouseMoved(e, &mouse.x, &mouse.y))
    {
        mouse = vec2_sub(cameraGetMousePosView(&scene->camera, mouse), editor->offset);

        if (editor->clicked)
            SetEntityPosition(&scene->ecs, editor->hover, grid_clip_vec2(scene->map.tile_size, mouse));
        else
            editor->hover = GetEntityAt(&scene->ecs, mouse);
    }
}

void WorldEditorOnUpdate(WorldEditor* editor, Scene* scene, float deltatime)
{

}

void WorldEditorOnRender(const WorldEditor* editor, Scene* scene, int show_grid, float padding)
{
    ignisPrimitivesRendererSetViewProjection(cameraGetViewProjectionPtr(&scene->camera));

    if (show_grid) SceneEditorRenderGrid(scene, padding);

    if (editor->hover != ECS_NULL_ENTITY)
    {
        rect r = GetEntityRect(&scene->ecs, editor->hover);

        vec2 position = GetEntityPosition(&scene->ecs, editor->hover);

        ignisPrimitives2DRenderRect(r.min.x, r.min.y, r.max.x - r.min.x, r.max.y - r.min.y, IGNIS_WHITE);
        ignisPrimitives2DRenderCircle(position.x, position.y, 2.0f, IGNIS_WHITE);
    }

    TileMapRenderDebug(&scene->map);

    ignisPrimitivesRendererFlush();
}
