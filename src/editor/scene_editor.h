#ifndef SCENE_EDITOR_H
#define SCENE_EDITOR_H

#include "Scenes/Scene.h"

#include "map_editor.h"
#include "world_editor.h"

typedef enum
{
    SCENE_EDIT_NONE,
    SCENE_EDIT_WORLD,
    SCENE_EDIT_MAP
} SceneEditMode;

typedef struct
{
    Scene* scene;

    int showgrid;

    float cameraspeed;
    float padding;
    
    SceneEditMode mode;
    WorldEditor world;
    MapEditor   map;
} SceneEditor;

void SceneEditorInit(SceneEditor* editor, Scene* scene, float cameraspeed, int padding, SceneEditMode mode);

void SceneEditorReset(SceneEditor* editor);

void SceneEditorSetMode(SceneEditor* editor, SceneEditMode mode);
void SceneEditorToggleWorldMode(SceneEditor* editor);
void SceneEditorToggleMapMode(SceneEditor* editor);
void SceneEditorToggleGrid(SceneEditor* editor);

int SceneEditorIsActive(const SceneEditor* editor);

void SceneEditorRenderGrid(const Scene* scene, float padding);

void SceneEditorOnEvent(SceneEditor* editor, const MinimalEvent* e);
void SceneEditorOnUpdate(SceneEditor* editor, float deltatime);
void SceneEditorOnRender(const SceneEditor* editor);

#endif /* !SCENE_EDITOR_H */
