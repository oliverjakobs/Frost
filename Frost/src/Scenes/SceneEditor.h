#ifndef SCENE_EDITOR_H
#define SCENE_EDITOR_H

#include "Scenes.h"

typedef struct
{
	int showgrid;
	int active;

	float cameraspeed;
	float gridsize;
	float padding;

	int clicked;

	vec2 offset;
	EntityID hover;
} SceneEditor;

void SceneEditorInit(SceneEditor* editor, float cameraspeed, float gridsize, int padding);

void SceneEditorReset(SceneEditor* editor);

void SceneEditorToggleActive(SceneEditor* editor);
void SceneEditorToggleGrid(SceneEditor* editor);

void SceneEditorOnEvent(SceneEditor* editor, Scenes* scene, Event e);
void SceneEditorOnUpdate(SceneEditor* editor, Scenes* scene, float deltatime);
void SceneEditorOnRender(SceneEditor* editor, Scenes* scene);

#endif /* !SCENE_EDITOR_H */
