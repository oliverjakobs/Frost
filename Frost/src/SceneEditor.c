#include "SceneEditor.h"

#include "Application/Debugger.h"
#include "Grid.h"

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
	editor->layer = 0;
	editor->clicked = 0;
	editor->offset = (vec2){ 0.0f, 0.0f };
	editor->hover = NULL;
}

void SceneEditorOnEvent(SceneEditor* editor, Scene* active, Event e)
{
	if (e.type == EVENT_KEY_PRESSED)
	{
		switch (e.key.keycode)
		{
		case KEY_UP:
			SceneEditorChangeLayer(editor, editor->layer + 1, active->max_layer);
			break;
		case KEY_DOWN:
			SceneEditorChangeLayer(editor, editor->layer - 1, active->max_layer);
			break;
		}
	}

	if (EventMouseButtonPressed(&e, MOUSE_BUTTON_LEFT))
	{
		if (editor->hover)
		{
			vec2 mouse = CameraGetMousePos(active->camera, InputMousePositionVec2());
			editor->offset = vec2_sub(mouse, EcsEntityGetPosition(editor->hover));
			editor->clicked = 1;
		}
	}

	if (EventMouseButtonReleased(&e, MOUSE_BUTTON_LEFT))
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
		EcsEntitySetPosition(editor->hover, GridClipVec2(editor->gridsize, vec2_sub(mouse, editor->offset)));
	else
		editor->hover = SceneGetEntityAt(active, mouse, editor->layer);
}

void SceneEditorOnRender(SceneEditor* editor, Scene* active)
{
	Primitives2DStart(CameraGetViewProjectionPtr(active->camera));

	/* render grid */
	if (editor->showgrid)
	{
		IgnisColorRGBA color = IGNIS_WHITE;
		ignisBlendColorRGBA(&color, 0.2f);

		GridRender(editor->gridsize, editor->padding, active->width, active->height, color);
	}

	clib_vector* layer = &active->layers[editor->layer];

	if (layer)
	{
		for (size_t i = 0; i < layer->size; i++)
		{
			IgnisColorRGBA color = IGNIS_WHITE;
			ignisBlendColorRGBA(&color, 0.4f);

			EcsTextureComponent* tex = layer_vector_get(layer, i)->texture;

			if (tex)
			{
				vec2 position = EcsEntityGetPosition(layer_vector_get(layer, i));

				vec2 min = vec2_sub(position, (vec2) { tex->width / 2.0f, 0.0f });
				vec2 max = vec2_add(min, (vec2) { tex->width, tex->height });

				Primitives2DRenderRect(min.x, min.y, max.x - min.x, max.y - min.y, color);
			}
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

void SceneEditorChangeLayer(SceneEditor* editor, int layer, int max_layer)
{
	if (layer < 0)
		editor->layer;
	else if (layer >= max_layer)
		editor->layer = max_layer - 1;
	else
		editor->layer = layer;
}