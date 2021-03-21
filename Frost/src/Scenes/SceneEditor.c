#include "SceneEditor.h"

#include "Frost/Frost.h"

#include "Application/Logger.h"

#include "MapEditor.h"
#include "WorldEditor.h"

void SceneEditorInit(SceneEditor* editor, Scene* scene, float cameraspeed, int padding, SceneEditMode mode)
{
	editor->scene = scene;
	editor->mode = mode;

	editor->cameraspeed = cameraspeed;
	editor->padding = editor->scene->map.tile_size * padding;

	editor->showgrid = 1;

	SceneEditorReset(editor);
}

void SceneEditorReset(SceneEditor* editor)
{
	editor->clicked = 0;
	editor->offset = vec2_zero();
	editor->hover = ECS_NULL_ENTITY;

	editor->tile_hover = NULL;
}

void SceneEditorSetMode(SceneEditor* editor, SceneEditMode mode)
{
	editor->mode = mode;
}

void SceneEditorToggleWorldMode(SceneEditor* editor)
{
	if (editor->mode == SCENE_EDIT_WORLD)
		editor->mode = SCENE_EDIT_NONE;
	else
		editor->mode = SCENE_EDIT_WORLD;
}

void SceneEditorToggleMapMode(SceneEditor* editor)
{
	if (editor->mode == SCENE_EDIT_MAP)
		editor->mode = SCENE_EDIT_NONE;
	else
		editor->mode = SCENE_EDIT_MAP;
}

void SceneEditorToggleGrid(SceneEditor* editor)
{
	editor->showgrid = !editor->showgrid;
}

int SceneEditorIsActive(const SceneEditor* editor)
{
	return editor->mode != SCENE_EDIT_NONE;
}

void SceneEditorRenderGrid(const SceneEditor* editor)
{
	if (!editor->showgrid) return;

	IgnisColorRGBA color = IGNIS_WHITE;
	ignisBlendColorRGBA(&color, 0.2f);

	float padding = editor->padding;
	float granularity = editor->scene->map.tile_size;

	for (float x = -padding; x <= SceneGetWidth(editor->scene) + padding; x += granularity)
		Primitives2DRenderLine(x, -padding, x, SceneGetHeight(editor->scene) + padding, color);

	for (float y = -padding; y <= SceneGetHeight(editor->scene) + padding; y += granularity)
		Primitives2DRenderLine(-padding, y, SceneGetWidth(editor->scene) + padding, y, color);
}

void SceneEditorOnEvent(SceneEditor* editor, Event e)
{
	if (editor->mode == SCENE_EDIT_WORLD)
		WorldEditorOnEvent(editor, e);
	else if (editor->mode == SCENE_EDIT_MAP)
		MapEditorOnEvent(editor, e);
}

void SceneEditorOnUpdate(SceneEditor* editor, float deltatime)
{
	if (editor->mode == SCENE_EDIT_NONE) return;

	vec3 position = editor->scene->camera.position;

	if (InputKeyPressed(KEY_A))
		position.x -= editor->cameraspeed * deltatime;
	if (InputKeyPressed(KEY_D))
		position.x += editor->cameraspeed * deltatime;
	if (InputKeyPressed(KEY_S))
		position.y -= editor->cameraspeed * deltatime;
	if (InputKeyPressed(KEY_W))
		position.y += editor->cameraspeed * deltatime;

	editor->scene->camera.position = position;
	CameraUpdateViewOrtho(&editor->scene->camera);

	if (editor->mode == SCENE_EDIT_WORLD)
		WorldEditorOnUpdate(editor, deltatime);
	else if (editor->mode == SCENE_EDIT_MAP)
		MapEditorOnUpdate(editor, deltatime);
}

void SceneEditorOnRender(const SceneEditor* editor)
{
	if (editor->mode == SCENE_EDIT_WORLD)
		WorldEditorOnRender(editor);
	else if (editor->mode == SCENE_EDIT_MAP)
		MapEditorOnRender(editor);
}