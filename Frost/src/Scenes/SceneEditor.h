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
	EntityID hover;
} SceneEditor;

void SceneEditorInit(SceneEditor* editor, float cameraspeed, float gridsize, float padding);

void SceneEditorReset(SceneEditor* editor);

void SceneEditorToggleActive(SceneEditor* editor);

void SceneEditorOnEvent(SceneEditor* editor, Ecs* ecs, Scene* active, Event e);
void SceneEditorOnUpdate(SceneEditor* editor, Ecs* ecs, Scene* active, float deltatime);
void SceneEditorOnRender(SceneEditor* editor, Ecs* ecs, Scene* active);

#endif /* !SCENE_EDITOR_H */
