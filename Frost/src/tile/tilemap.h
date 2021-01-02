#ifndef TILEMAP_H
#define TILEMAP_H

#include "tile.h"

typedef struct
{
	size_t width;
	size_t height;

	float tile_size;

	Tile* tiles;

	/* types */
	TileType* types;
	size_t types_count;
} TileMap;

int TileMapLoad(TileMap* map, TileID* tiles, size_t width, size_t height, float tile_size, TileType* types, size_t types_count);
void TileMapDestroy(TileMap* map);

Tile* TileMapAt(const TileMap* map, size_t row, size_t col);
Tile* TileMapAtPos(const TileMap* map, vec2 pos);

int TileMapCheckType(const TileMap* map, vec2 pos, TileType type);

TileType TileMapGetType(const TileMap* map, TileID id);

int32_t TileMapClamp(const TileMap* map, float x);

#endif /* !TILEMAP_H */
