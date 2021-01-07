#include "SceneEditor.h"

#include "Frost/Frost.h"

#include "Application/Debugger.h"
#include "math/grid.h"

void SceneEditorInit(SceneEditor* editor, float cameraspeed, float gridsize, int padding)
{
	editor->cameraspeed = cameraspeed;
	editor->gridsize = gridsize;
	editor->padding = gridsize * padding;

	editor->showgrid = 1;
	editor->active = 0;

	SceneEditorReset(editor);
}

void SceneEditorReset(SceneEditor* editor)
{
	editor->clicked = 0;
	editor->offset = vec2_zero();
	editor->hover = ECS_NULL_ENTITY;
}

void SceneEditorToggleActive(SceneEditor* editor)
{
	editor->active = !editor->active;
}

void SceneEditorToggleGrid(SceneEditor* editor)
{
	editor->showgrid = !editor->showgrid;
}

void SceneEditorOnEvent(SceneEditor* editor, Scene* scene, Event e)
{
	if (!editor->active) return;

	if (EventMouseButtonPressed(&e) == MOUSE_BUTTON_LEFT)
	{
		if (editor->hover != ECS_NULL_ENTITY)
		{
			vec2 mouse = CameraGetMousePosView(scene->camera, InputMousePositionVec2());
			editor->offset = vec2_sub(mouse, GetEntityPosition(&scene->ecs, editor->hover));
			editor->clicked = 1;
		}
	}

	if (EventMouseButtonReleased(&e) == MOUSE_BUTTON_LEFT)
	{
		editor->offset = vec2_zero();
		editor->clicked = 0;
	}
}

void SceneEditorOnUpdate(SceneEditor* editor, Scene* scene, float deltatime)
{
	if (!editor->active) return;

	vec3 position = scene->camera->position;

	if (InputKeyPressed(KEY_A))
		position.x -= editor->cameraspeed * deltatime;
	if (InputKeyPressed(KEY_D))
		position.x += editor->cameraspeed * deltatime;
	if (InputKeyPressed(KEY_S))
		position.y -= editor->cameraspeed * deltatime;
	if (InputKeyPressed(KEY_W))
		position.y += editor->cameraspeed * deltatime;

	scene->camera->position = position;
	CameraUpdateViewOrtho(scene->camera);

	vec2 mouse = CameraGetMousePosView(scene->camera, InputMousePositionVec2());

	if (editor->clicked)
		SetEntityPosition(&scene->ecs, editor->hover, grid_clip_vec2(editor->gridsize, vec2_sub(mouse, editor->offset)));
	else
		editor->hover = GetEntityAt(&scene->ecs, mouse);
}

void SceneEditorOnRender(SceneEditor* editor, Scene* scene)
{
	if (!editor->active) return;

	Primitives2DStart(CameraGetViewProjectionPtr(scene->camera));

	/* render grid */
	if (editor->showgrid)
	{
		IgnisColorRGBA color = IGNIS_WHITE;
		ignisBlendColorRGBA(&color, 0.2f);

		float padding = editor->padding;
		float granularity = editor->gridsize;

		for (float x = -padding; x <= SceneGetWidth(scene) + padding; x += granularity)
			Primitives2DRenderLine(x, -padding, x, SceneGetHeight(scene) + padding, color);

		for (float y = -padding; y <= SceneGetHeight(scene) + padding; y += granularity)
			Primitives2DRenderLine(-padding, y, SceneGetWidth(scene) + padding, y, color);
	}

	if (editor->hover != ECS_NULL_ENTITY)
	{
		rect r = GetEntityRect(&scene->ecs, editor->hover);

		vec2 position = GetEntityPosition(&scene->ecs, editor->hover);

		Primitives2DRenderRect(r.min.x, r.min.y, r.max.x - r.min.x, r.max.y - r.min.y, IGNIS_WHITE);
		Primitives2DRenderCircle(position.x, position.y, 2.0f, IGNIS_WHITE);
	}

	TileMapRenderDebug(&scene->map);

	Primitives2DFlush();
}