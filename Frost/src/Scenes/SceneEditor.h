#ifndef SCENE_EDITOR_H
#define SCENE_EDITOR_H

#include "Scene.h"

typedef enum
{
	SCENE_EDIT_NONE,
	SCENE_EDIT_WORLD,
	SCENE_EDIT_MAP
} SceneEditMode;

typedef struct
{
	SceneEditMode mode;
	Scene* scene;

	int showgrid;

	float cameraspeed;
	float gridsize;
	float padding;

	int clicked;

	vec2 offset;
	EcsEntityID hover;
} SceneEditor;

void SceneEditorInit(SceneEditor* editor, Scene* scene, float cameraspeed, float gridsize, int padding);

void SceneEditorReset(SceneEditor* editor);

void SceneEditorSetMode(SceneEditor* editor, SceneEditMode mode);
void SceneEditorToggleWorldMode(SceneEditor* editor);
void SceneEditorToggleMapMode(SceneEditor* editor);
void SceneEditorToggleGrid(SceneEditor* editor);

int SceneEditorIsActive(const SceneEditor* editor);

void SceneEditorOnEvent(SceneEditor* editor, Event e);
void SceneEditorOnUpdate(SceneEditor* editor, float deltatime);
void SceneEditorOnRender(const SceneEditor* editor);

#endif /* !SCENE_EDITOR_H */
