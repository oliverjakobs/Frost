#include "MapEditor.h"

#define PALETTE_TILE_SIZE	32.0f
#define PALETTE_PADDING		20.0f
#define PALETTE_HEIGHT		((2 * PALETTE_PADDING) + PALETTE_TILE_SIZE)

/* TODO insertion mode: REPLACE, INSERT(if not empty), DELETE, DELETEMATCH(if tile matches selected delete it) */
void MapEditorOnEvent(SceneEditor* editor, Event e)
{
}

void MapEditorOnUpdate(SceneEditor* editor, float deltatime)
{
	vec2 mouse = CameraGetMousePosView(&editor->scene->camera, InputMousePositionVec2());

	if (mouse.y > (editor->scene->camera.position.y + PALETTE_HEIGHT))
	{
		editor->tile_hover = TileMapAtPos(&editor->scene->map, mouse);

		if (editor->tile_hover && InputMousePressed(MOUSE_BUTTON_LEFT))
		{
			TileMapSetAtPos(&editor->scene->map, mouse, editor->palette_select);
			TileRendererUpdateBuffers(&editor->scene->renderer, &editor->scene->map);
		}
	}
	else
	{
		editor->tile_hover = NULL;
		
		mouse.x -= PALETTE_PADDING;
		mouse.y -= PALETTE_PADDING;
		if (mouse.x >= 0.0f && mouse.x <= (editor->scene->map.types_count * PALETTE_TILE_SIZE) &&
			mouse.y >= 0.0f && mouse.y <= PALETTE_TILE_SIZE)
		{
			editor->palette_hover = (int)floorf(mouse.x / PALETTE_TILE_SIZE);
		}
		else
		{
			editor->palette_hover = -1;
		}

		if (editor->palette_hover >= 0 && InputMousePressed(MOUSE_BUTTON_LEFT))
			editor->palette_select = editor->palette_hover;
	}
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
		float y = PALETTE_PADDING;
		float w = PALETTE_TILE_SIZE;
		float h = PALETTE_TILE_SIZE;

		float src_x, src_y, src_w, src_h;
		GetTexture2DSrcRect(&editor->scene->tile_set, i, &src_x, &src_y, &src_w, &src_h);

		BatchRenderer2DRenderTextureSrc(&editor->scene->tile_set, x, y, w, h, src_x, src_y, src_w, src_h);
	}

	BatchRenderer2DFlush();

	/* UI Overlay */
	Primitives2DStart(CameraGetProjectionPtr(&editor->scene->camera));

	if (editor->palette_hover >= 0)
	{
		float x = PALETTE_PADDING + (editor->palette_hover * PALETTE_TILE_SIZE);
		float y = PALETTE_PADDING;

		IgnisColorRGBA color = IGNIS_WHITE;
		ignisBlendColorRGBA(&color, 0.2f);

		Primitives2DFillRect(x, y, PALETTE_TILE_SIZE, PALETTE_TILE_SIZE, color);
	}

	if (editor->palette_select >= 0)
	{
		float x = PALETTE_PADDING + (editor->palette_select * PALETTE_TILE_SIZE);
		float y = PALETTE_PADDING;

		Primitives2DRenderRect(x, y, PALETTE_TILE_SIZE, PALETTE_TILE_SIZE, IGNIS_WHITE);
	}

	Primitives2DFlush();
}
