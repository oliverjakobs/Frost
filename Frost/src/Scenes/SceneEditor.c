#include "SceneEditor.h"

#include "Application/Debugger.h"
#include "math/grid.h"

void SceneEditorInit(SceneEditor* editor, float cameraspeed, float gridsize, float padding)
{
	editor->cameraspeed = cameraspeed;
	editor->gridsize = gridsize;
	editor->padding = padding;

	SceneEditorReset(editor);
}

void SceneEditorReset(SceneEditor* editor)
{
	editor->showgrid = 1;
	editor->clicked = 0;
	editor->offset = (vec2){ 0.0f, 0.0f };
	editor->hover = NULL;
}

void SceneEditorOnEvent(SceneEditor* editor, Scene* active, Event e)
{
	if (EventMouseButtonPressed(&e) == MOUSE_BUTTON_LEFT)
	{
		if (editor->hover)
		{
			vec2 mouse = CameraGetMousePos(active->camera, InputMousePositionVec2());
			editor->offset = vec2_sub(mouse, EcsEntityGetPosition(editor->hover));
			editor->clicked = 1;
		}
	}

	if (EventMouseButtonReleased(&e) == MOUSE_BUTTON_LEFT)
	{
		editor->offset = (vec2){ 0.0f,0.0f };
		editor->clicked = 0;
	}
}

void SceneEditorOnUpdate(SceneEditor* editor, Scene* active, float deltatime)
{
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
		EcsEntitySetPosition(editor->hover, grid_clip_vec2(editor->gridsize, vec2_sub(mouse, editor->offset)));
	else
		editor->hover = SceneGetEntityAt(active, mouse);
}

void SceneEditorOnRender(SceneEditor* editor, Scene* active)
{
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

	for (size_t i = 0; i < active->entities.used; ++i)
	{
		IgnisColorRGBA color = IGNIS_WHITE;
		ignisBlendColorRGBA(&color, 0.4f);

		EcsEntity* entity = (EcsEntity*)clib_array_get(&active->entities, i);

		if (entity->texture)
		{
			vec2 position = EcsEntityGetPosition(entity);

			vec2 min = vec2_sub(position, (vec2) { entity->texture->width / 2.0f, 0.0f });
			vec2 max = vec2_add(min, (vec2) { entity->texture->width, entity->texture->height });

			Primitives2DRenderRect(min.x, min.y, max.x - min.x, max.y - min.y, color);
		}
	}

	if (editor->hover)
	{
		EcsTextureComponent* tex = editor->hover->texture;

		if (tex)
		{
			vec2 position = EcsEntityGetPosition(editor->hover);

			vec2 min = vec2_sub(position, (vec2) { tex->width / 2.0f, 0.0f });
			vec2 max = vec2_add(min, (vec2) { tex->width, tex->height });

			Primitives2DRenderRect(min.x, min.y, max.x - min.x, max.y - min.y, IGNIS_WHITE);
			Primitives2DRenderCircle(position.x, position.y, 2.0f, IGNIS_WHITE);
		}
	}

	Primitives2DFlush();
}