#ifndef TILEMAP_H
#define TILEMAP_H

#include "tile.h"
#include "Ignis/Ignis.h"

typedef struct
{
	size_t width;
	size_t height;

	float tile_size;

	Tile* tiles;
} TileMap;

int TileMapLoad(TileMap* map, TileID* tiles, size_t width, size_t height, float tile_size);
void TileMapDestroy(TileMap* map);

int32_t TileMapGetIndex(const TileMap* map, vec2 pos);
Tile* TileMapAt(const TileMap* map, size_t row, size_t col);

#endif /* !TILEMAP_H */
