#ifndef SCENE_EDITOR_H
#define SCENE_EDITOR_H

#include "SceneManager.h"

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

void SceneEditorInit(SceneEditor* editor, float cameraspeed, float gridsize, float padding);

void SceneEditorReset(SceneEditor* editor);

void SceneEditorToggleActive(SceneEditor* editor);

void SceneEditorOnEvent(SceneEditor* editor, SceneManager* scene, Event e);
void SceneEditorOnUpdate(SceneEditor* editor, SceneManager* scene, float deltatime);
void SceneEditorOnRender(SceneEditor* editor, SceneManager* scene);

#endif /* !SCENE_EDITOR_H */
