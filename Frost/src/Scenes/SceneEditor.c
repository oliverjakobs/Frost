#include "SceneEditor.h"

#include "Application/Debugger.h"
#include "math/grid.h"

void SceneEditorInit(SceneEditor* editor, float cameraspeed, float gridsize, float padding)
{
	editor->cameraspeed = cameraspeed;
	editor->gridsize = gridsize;
	editor->padding = padding;

	editor->showgrid = 1;
	editor->active = 0;

	SceneEditorReset(editor);
}

void SceneEditorReset(SceneEditor* editor)
{
	editor->clicked = 0;
	editor->offset = vec2_zero();
	editor->hover = NULL_ENTITY;
}

void SceneEditorToggleActive(SceneEditor* editor)
{
	editor->active = !editor->active;
}

void SceneEditorOnEvent(SceneEditor* editor, Ecs* ecs, Scene* active, Event e)
{
	if (!editor->active) return;

	if (EventMouseButtonPressed(&e) == MOUSE_BUTTON_LEFT)
	{
		if (editor->hover != NULL_ENTITY)
		{
			vec2 mouse = CameraGetMousePos(active->camera, InputMousePositionVec2());
			editor->offset = vec2_sub(mouse, EcsGetEntityPosition(ecs, editor->hover));
			editor->clicked = 1;
		}
	}

	if (EventMouseButtonReleased(&e) == MOUSE_BUTTON_LEFT)
	{
		editor->offset = vec2_zero();
		editor->clicked = 0;
	}
}

void SceneEditorOnUpdate(SceneEditor* editor, Ecs* ecs, Scene* active, float deltatime)
{
	if (!editor->active) return;

	vec3 position = active->camera->position;

	if (InputKeyPressed(KEY_A))
		position.x -= editor->cameraspeed * deltatime;
	if (InputKeyPressed(KEY_D))
		position.x += editor->cameraspeed * deltatime;
	if (InputKeyPressed(KEY_S))
		position.y -= editor->cameraspeed * deltatime;
	if (InputKeyPressed(KEY_W))
		position.y += editor->cameraspeed * deltatime;

	active->camera->position = position;
	CameraUpdateViewOrtho(active->camera);

	vec2 mouse = CameraGetMousePos(active->camera, InputMousePositionVec2());

	if (editor->clicked)
		EcsSetEntityPosition(ecs, editor->hover, grid_clip_vec2(editor->gridsize, vec2_sub(mouse, editor->offset)));
	else
		editor->hover = EcsGetEntityAt(ecs, mouse);
}

void SceneEditorOnRender(SceneEditor* editor, Ecs* ecs, Scene* active)
{
	if (!editor->active) return;

	Primitives2DStart(CameraGetViewProjectionPtr(active->camera));

	/* render grid */
	if (editor->showgrid)
	{
		IgnisColorRGBA color = IGNIS_WHITE;
		ignisBlendColorRGBA(&color, 0.2f);

		float padding = editor->padding;
		float granularity = editor->gridsize;
		float width = active->width;
		float height = active->height;

		for (float x = -padding; x <= width + padding; x += granularity)
			Primitives2DRenderLine(x, -padding, x, height + padding, color);

		for (float y = -padding; y <= height + padding; y += granularity)
			Primitives2DRenderLine(-padding, y, width + padding, y, color);
	}

	if (editor->hover != NULL_ENTITY)
	{
		rect r = EcsGetEntityRect(ecs, editor->hover);

		vec2 position = EcsGetEntityPosition(ecs, editor->hover);

		Primitives2DRenderRect(r.min.x, r.min.y, r.max.x - r.min.x, r.max.y - r.min.y, IGNIS_WHITE);
		Primitives2DRenderCircle(position.x, position.y, 2.0f, IGNIS_WHITE);
	}

	Primitives2DFlush();
}