#include "tilemap.h"

#include <stdlib.h>

#include "Graphics/Renderer.h"

#include <assert.h>

int TileMapLoad(TileMap* map, TileID* tiles, size_t width, size_t height, float tile_size)
{
    map->width = width;
    map->height = height;
    map->tile_size = tile_size;

    map->tiles = malloc(width * height * sizeof(Tile));

	if (!map->tiles) return 0;

	size_t index = 0;

	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			Tile tile;
			tile.pos = (vec2){ col * tile_size, (height - (row + 1)) * tile_size };
			tile.id = tiles[row * width + col];

			tile.type = tile.id > 0 ? TILE_SOLID : TILE_EMPTY;

			map->tiles[index++] = tile;
		}
	}

	return 1;
}

void TileMapDestroy(TileMap* map)
{
	free(map->tiles);
}

Tile* TileMapAt(const TileMap* map, size_t row, size_t col)
{
	if (row < 0 || col < 0) return NULL;
	if (row >= map->height || col >= map->width) return NULL;

    return &map->tiles[(map->height - row - 1) * map->width + col];
}

Tile* TileMapAtPos(const TileMap* map, vec2 pos)
{
	size_t col = TileMapClamp(map, pos.x);
	size_t row = TileMapClamp(map, pos.y);

	return TileMapAt(map, row, col);
}

int TileMapCheckType(const TileMap* map, vec2 pos, TileType type)
{
	Tile* tile = TileMapAtPos(map, pos);
	return tile && tile->type == type;
}

int32_t TileMapClamp(const TileMap* map, float x) { return (int32_t)floorf(x / map->tile_size); }
