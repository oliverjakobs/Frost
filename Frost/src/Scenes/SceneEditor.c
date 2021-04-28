#include "SceneEditor.h"

#include "Frost/Frost.h"

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
	if (editor->mode == SCENE_EDIT_WORLD)		WorldEditorReset(&editor->world);
	else if (editor->mode == SCENE_EDIT_MAP)	MapEditorReset(&editor->map);
}

void SceneEditorSetMode(SceneEditor* editor, SceneEditMode mode)
{
	editor->mode = mode;
	SceneEditorReset(editor);
}

void SceneEditorToggleWorldMode(SceneEditor* editor)
{
	if (editor->mode == SCENE_EDIT_WORLD)	editor->mode = SCENE_EDIT_NONE;
	else									editor->mode = SCENE_EDIT_WORLD;
	SceneEditorReset(editor);
}

void SceneEditorToggleMapMode(SceneEditor* editor)
{
	if (editor->mode == SCENE_EDIT_MAP)		editor->mode = SCENE_EDIT_NONE;
	else									editor->mode = SCENE_EDIT_MAP;
	SceneEditorReset(editor);
}

void SceneEditorToggleGrid(SceneEditor* editor)
{
	editor->showgrid = !editor->showgrid;
}

int SceneEditorIsActive(const SceneEditor* editor)
{
	return editor->mode != SCENE_EDIT_NONE;
}

void SceneEditorRenderGrid(const Scene* scene, float padding)
{
	IgnisColorRGBA color = IGNIS_WHITE;
	ignisBlendColorRGBA(&color, 0.2f);

	float granularity = scene->map.tile_size;

	for (float x = -padding; x <= SceneGetWidth(scene) + padding; x += granularity)
		Primitives2DRenderLine(x, -padding, x, SceneGetHeight(scene) + padding, color);

	for (float y = -padding; y <= SceneGetHeight(scene) + padding; y += granularity)
		Primitives2DRenderLine(-padding, y, SceneGetWidth(scene) + padding, y, color);
}

void SceneEditorOnEvent(SceneEditor* editor, Event e)
{
	if (editor->mode == SCENE_EDIT_WORLD)		WorldEditorOnEvent(&editor->world, editor->scene, e);
	else if (editor->mode == SCENE_EDIT_MAP)	MapEditorOnEvent(&editor->map, editor->scene, e);
}

void SceneEditorOnUpdate(SceneEditor* editor, float deltatime)
{
	if (editor->mode == SCENE_EDIT_NONE) return;

	vec2 position = editor->scene->camera.position;

	if (MinimalKeyDown(MINIMAL_KEY_A))	position.x -= editor->cameraspeed * deltatime;
	if (MinimalKeyDown(MINIMAL_KEY_D))	position.x += editor->cameraspeed * deltatime;
	if (MinimalKeyDown(MINIMAL_KEY_S))	position.y -= editor->cameraspeed * deltatime;
	if (MinimalKeyDown(MINIMAL_KEY_W))	position.y += editor->cameraspeed * deltatime;

	CameraSetPositionOrtho(&editor->scene->camera, position);

	if (editor->mode == SCENE_EDIT_WORLD)		WorldEditorOnUpdate(&editor->world, editor->scene, deltatime);
	else if (editor->mode == SCENE_EDIT_MAP)	MapEditorOnUpdate(&editor->map, editor->scene, deltatime);
}

void SceneEditorOnRender(const SceneEditor* editor)
{
	if (editor->mode == SCENE_EDIT_WORLD)
		WorldEditorOnRender(&editor->world, editor->scene, editor->showgrid, editor->padding);
	else if (editor->mode == SCENE_EDIT_MAP)
		MapEditorOnRender(&editor->map, editor->scene, editor->showgrid, editor->padding);
}