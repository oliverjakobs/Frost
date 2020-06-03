#ifndef SCENE_EDITOR_H
#define SCENE_EDITOR_H

#include "Scene.h"

typedef struct
{
	int showgrid;
	int layer;

	float cameraspeed;
	float gridsize;
	float padding;

	int clicked;

	vec2 offset;
	EcsEntity* hover;
} SceneEditor;

void SceneEditorInit(SceneEditor* editor, float cameraspeed, float gridsize, float padding);

void SceneEditorReset(SceneEditor* editor);

void SceneEditorOnEvent(SceneEditor* editor, Scene* active, Event e);
void SceneEditorOnUpdate(SceneEditor* editor, Scene* active, float deltatime);
void SceneEditorOnRender(SceneEditor* editor, Scene* active);

void SceneEditorChangeLayer(SceneEditor* editor, int layer, size_t max_layer);

#endif /* !SCENE_EDITOR_H */
