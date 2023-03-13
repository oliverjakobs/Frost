#ifndef WORLD_EDITOR_H
#define WORLD_EDITOR_H

#include "Scenes/Scene.h"

typedef struct
{
    vec2 offset;
    EcsEntityID hover;
    int clicked;
} WorldEditor;

void WorldEditorReset(WorldEditor* editor);

void WorldEditorOnEvent(WorldEditor* editor, Scene* scene, const MinimalEvent* e);
void WorldEditorOnUpdate(WorldEditor* editor, Scene* scene, float deltatime);
void WorldEditorOnRender(const WorldEditor* editor, Scene* scene, int show_grid, float padding);

#endif // !WORLD_EDITOR_H