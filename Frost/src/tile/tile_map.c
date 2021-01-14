#include "tile_map.h"

#include <stdlib.h>

int TileMapLoad(TileMap* map, TileID* tiles, size_t rows, size_t cols, float tile_size, TileType* types, size_t types_count)
{
    map->rows = rows;
    map->cols = cols;
    map->tile_size = tile_size;
	map->types = types;
	map->types_count = types_count;

	map->borders[TILE_LEFT] = 0;
	map->borders[TILE_RIGHT] = 0;
	map->borders[TILE_TOP] = 0;
	map->borders[TILE_BOTTOM] = 0;

    map->tiles = malloc(rows * cols * sizeof(Tile));

	if (!map->tiles) return 0;

	size_t index = 0;
	for (size_t row = 0; row < rows; ++row)
	{
		for (size_t col = 0; col < cols; ++col)
		{
			Tile tile;
			tile.pos = (vec2){ col * tile_size, (rows - (row + 1)) * tile_size };
			tile.id = tiles[row * cols + col];

			tile.type = TileMapGetType(map, tile.id);

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
	if (row >= map->rows || col >= map->cols) return NULL;

    return &map->tiles[(map->rows - row - 1) * map->cols + col];
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

TileType TileMapGetType(const TileMap* map, TileID id)
{
	if (id < 0 || id >= map->types_count) return TILE_EMPTY;

	return map->types[id];
}

int32_t TileMapClamp(const TileMap* map, float x) { return (int32_t)floorf(x / map->tile_size); }

void TileMapSetBorder(TileMap* map, TileDirection dir, int8_t value)
{
	map->borders[dir] = value;
}

int8_t TileMapGetBorder(const TileMap* map, TileDirection dir)
{
	return map->borders[dir];
}
