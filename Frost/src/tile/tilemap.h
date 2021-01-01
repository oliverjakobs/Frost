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

Tile* TileMapAt(const TileMap* map, size_t row, size_t col);
Tile* TileMapAtPos(const TileMap* map, vec2 pos);

int TileMapCheckType(const TileMap* map, vec2 pos, TileType type);

int32_t TileMapClamp(const TileMap* map, float x);

#endif /* !TILEMAP_H */
