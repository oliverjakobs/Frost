#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "Scene.h"

typedef struct
{
	const Tile* tile_hover;
	TileSetMode set_mode;
	int palette_hover;
	int palette_select;
} MapEditor;

void MapEditorReset(MapEditor* editor);

void MapEditorOnEvent(MapEditor* editor, Scene* scene, const MinimalEvent* e);
void MapEditorOnUpdate(MapEditor* editor, Scene* scene, float deltatime);
void MapEditorOnRender(const MapEditor* editor, Scene* scene, int show_grid, float padding);

#endif // !MAP_EDITOR_H
