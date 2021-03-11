#include "WorldEditor.h"

#include "Frost/Frost.h"
#include "math/grid.h"

void WorldEditorOnEvent(SceneEditor* editor, Event e)
{
	if (EventMouseButtonPressed(&e) == MOUSE_BUTTON_LEFT)
	{
		if (editor->hover != ECS_NULL_ENTITY)
		{
			vec2 mouse = CameraGetMousePosView(&editor->scene->camera, InputMousePositionVec2());
			editor->offset = vec2_sub(mouse, GetEntityPosition(&editor->scene->ecs, editor->hover));
			editor->clicked = 1;
		}
	}

	if (EventMouseButtonReleased(&e) == MOUSE_BUTTON_LEFT)
	{
		editor->offset = vec2_zero();
		editor->clicked = 0;
	}
}

void WorldEditorOnUpdate(SceneEditor* editor, float deltatime)
{
	vec2 mouse = CameraGetMousePosView(&editor->scene->camera, InputMousePositionVec2());

	if (editor->clicked)
		SetEntityPosition(&editor->scene->ecs, editor->hover, grid_clip_vec2(editor->gridsize, vec2_sub(mouse, editor->offset)));
	else
		editor->hover = GetEntityAt(&editor->scene->ecs, mouse);
}

void WorldEditorOnRender(const SceneEditor* editor)
{
	Primitives2DStart(CameraGetViewProjectionPtr(&editor->scene->camera));

	/* render grid */
	if (editor->showgrid)
	{
		IgnisColorRGBA color = IGNIS_WHITE;
		ignisBlendColorRGBA(&color, 0.2f);

		float padding = editor->padding;
		float granularity = editor->gridsize;

		for (float x = -padding; x <= SceneGetWidth(editor->scene) + padding; x += granularity)
			Primitives2DRenderLine(x, -padding, x, SceneGetHeight(editor->scene) + padding, color);

		for (float y = -padding; y <= SceneGetHeight(editor->scene) + padding; y += granularity)
			Primitives2DRenderLine(-padding, y, SceneGetWidth(editor->scene) + padding, y, color);
	}

	if (editor->hover != ECS_NULL_ENTITY)
	{
		rect r = GetEntityRect(&editor->scene->ecs, editor->hover);

		vec2 position = GetEntityPosition(&editor->scene->ecs, editor->hover);

		Primitives2DRenderRect(r.min.x, r.min.y, r.max.x - r.min.x, r.max.y - r.min.y, IGNIS_WHITE);
		Primitives2DRenderCircle(position.x, position.y, 2.0f, IGNIS_WHITE);
	}

	TileMapRenderDebug(&editor->scene->map);

	Primitives2DFlush();
}
