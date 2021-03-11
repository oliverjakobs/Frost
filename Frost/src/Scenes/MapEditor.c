#include "MapEditor.h"

const float PALETTE_HEIGHT = 100.0f;

void MapEditorOnEvent(SceneEditor* editor, Event e)
{
}

void MapEditorOnUpdate(SceneEditor* editor, float deltatime)
{
	vec2 mouse = CameraGetMousePosView(&editor->scene->camera, InputMousePositionVec2());

	if (mouse.y > (editor->scene->camera.position.y + PALETTE_HEIGHT))
		editor->tile_hover = TileMapAtPos(&editor->scene->map, mouse);
	else
		editor->tile_hover = NULL;
}

void MapEditorOnRender(const SceneEditor* editor)
{
	Primitives2DStart(CameraGetViewProjectionPtr(&editor->scene->camera));

	SceneEditorRenderGrid(editor);

	if (editor->tile_hover)
	{
		vec2 pos = editor->tile_hover->pos;
		float tile_size = editor->scene->map.tile_size;

		Primitives2DRenderRect(pos.x,pos.y, tile_size, tile_size, IGNIS_WHITE);
	}

	TileMapRenderDebug(&editor->scene->map);

	Primitives2DFlush();

	/* UI */
	Primitives2DStart(CameraGetProjectionPtr(&editor->scene->camera));

	Primitives2DFillRect(0.0f, 0.0f, editor->scene->camera.size.x, PALETTE_HEIGHT, IGNIS_GREEN);

	Primitives2DFlush();
}
