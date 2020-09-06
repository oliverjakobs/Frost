#ifndef SCENE_EDITOR_H
#define SCENE_EDITOR_H

#include "Scene.h"

typedef struct
{
	int showgrid;
	int active;

	float cameraspeed;
	float gridsize;
	float padding;

	int clicked;

	vec2 offset;
	EcsEntityID hover;
} SceneEditor;

void SceneEditorInit(SceneEditor* editor, float cameraspeed, float gridsize, int padding);

void SceneEditorReset(SceneEditor* editor);

void SceneEditorToggleActive(SceneEditor* editor);
void SceneEditorToggleGrid(SceneEditor* editor);

void SceneEditorOnEvent(SceneEditor* editor, Scene* scene, Event e);
void SceneEditorOnUpdate(SceneEditor* editor, Scene* scene, float deltatime);
void SceneEditorOnRender(SceneEditor* editor, Scene* scene);

#endif /* !SCENE_EDITOR_H */
