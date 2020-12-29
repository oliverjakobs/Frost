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

    return &map->tiles[row * map->width + col];
}

Tile* TileMapAtPos(const TileMap* map, vec2 pos)
{
	size_t col = TileMapGetClamp(map, pos.x);
	size_t row = TileMapGetClamp(map, pos.y);

	return TileMapAt(map, row, col);
}

int TileMapCheckType(const TileMap* map, vec2 pos, TileType type)
{
	Tile* tile = TileMapAtPos(map, pos);

	return tile && tile->type == type;
}

size_t TileMapGetArea(const TileMap* map, Tile** tiles, float x, float y, float w, float h)
{
	size_t start_col = TileMapGetClamp(map, x);
	size_t end_col = TileMapGetClamp(map, x + w);

	size_t start_row = TileMapGetClamp(map, y);
	size_t end_row = TileMapGetClamp(map, y + h);

	size_t tile_index = 0;
	for (size_t col = start_col; col <= end_col; ++col)
	{
		for (size_t row = start_row; row <= end_row; ++row)
		{
			Tile* tile = TileMapAt(map, row, col);

			if (tile && tile->type != TILE_EMPTY)
				tiles[tile_index++] = tile;
		}
	}

	return tile_index;
}

size_t TileMapGetClamp(const TileMap* map, float x) { return (size_t)floor(x / map->tile_size); }
