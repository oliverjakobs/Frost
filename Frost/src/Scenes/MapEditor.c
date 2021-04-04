#include "MapEditor.h"

const float PALETTE_HEIGHT = 100.0f;
const float PALETTE_TILE_SIZE = 32.0f;
const float PALETTE_PADDING = 20.0f;

/* TODO insertion mode: REPLACE, INSERT(if not empty), DELETE, DELETEMATCH(if tile matches selected delete it) */
void MapEditorOnEvent(SceneEditor* editor, Event e)
{
}

void MapEditorOnUpdate(SceneEditor* editor, float deltatime)
{
	vec2 mouse = CameraGetMousePosView(&editor->scene->camera, InputMousePositionVec2());

	if (mouse.y > (editor->scene->camera.position.y + PALETTE_HEIGHT))
		editor->tile_hover = TileMapAtPos(&editor->scene->map, mouse);
	else
	{
		editor->tile_hover = NULL;
		
		/* TODO select palette with mouse */
	}

	if (InputKeyPressed(KEY_1)) editor->palette_select = 0;
	if (InputKeyPressed(KEY_2)) editor->palette_select = 1;
	if (InputKeyPressed(KEY_3)) editor->palette_select = 2;
	if (InputKeyPressed(KEY_4)) editor->palette_select = 3;
	if (InputKeyPressed(KEY_5)) editor->palette_select = 4;
	if (InputKeyPressed(KEY_6)) editor->palette_select = 5;
	if (InputKeyPressed(KEY_7)) editor->palette_select = 6;
	if (InputKeyPressed(KEY_8)) editor->palette_select = 7;
	if (InputKeyPressed(KEY_9)) editor->palette_select = 8;
	if (InputKeyPressed(KEY_0)) editor->palette_select = 9;
}

void MapEditorOnRender(const SceneEditor* editor)
{
	Primitives2DStart(CameraGetViewProjectionPtr(&editor->scene->camera));

	SceneEditorRenderGrid(editor);
	TileMapRenderDebug(&editor->scene->map);

	if (editor->tile_hover)
	{
		vec2 pos = editor->tile_hover->pos;
		float tile_size = editor->scene->map.tile_size;

		Primitives2DRenderRect(pos.x,pos.y, tile_size, tile_size, IGNIS_GREEN);
	}

	/* UI */

	Primitives2DFillRect(0.0f, 0.0f, editor->scene->camera.size.x, PALETTE_HEIGHT, IGNIS_LIGHT_GREY);

	Primitives2DFlush();

	/* Palette */
	BatchRenderer2DStart(CameraGetViewProjectionPtr(&editor->scene->camera));

	for (size_t i = 0; i < editor->scene->map.types_count; ++i)
	{
		float x = PALETTE_PADDING + (i * PALETTE_TILE_SIZE);
		float y = PALETTE_HEIGHT - (PALETTE_TILE_SIZE + PALETTE_PADDING);
		float w = PALETTE_TILE_SIZE;
		float h = PALETTE_TILE_SIZE;

		float src_x, src_y, src_w, src_h;
		GetTexture2DSrcRect(&editor->scene->tile_set, i, &src_x, &src_y, &src_w, &src_h);

		BatchRenderer2DRenderTextureSrc(&editor->scene->tile_set, x, y, w, h, src_x, src_y, src_w, src_h);
	}

	BatchRenderer2DFlush();

	/* UI Overlay */
	Primitives2DStart(CameraGetProjectionPtr(&editor->scene->camera));

	if (editor->palette_select >= 0)
	{
		float x = PALETTE_PADDING + (editor->palette_select * PALETTE_TILE_SIZE);
		float y = PALETTE_HEIGHT - (PALETTE_TILE_SIZE + PALETTE_PADDING);

		IgnisColorRGBA color = IGNIS_WHITE;
		ignisBlendColorRGBA(&color, 0.2f);

		Primitives2DFillRect(x, y, PALETTE_TILE_SIZE, PALETTE_TILE_SIZE, color);
	}

	Primitives2DFlush();
}
