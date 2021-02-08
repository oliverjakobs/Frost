#include "tile_map.h"

#include <stdlib.h>

int TileMapLoad(TileMap* map, size_t rows, size_t cols, float tile_size)
{
    map->rows = rows;
    map->cols = cols;
    map->tile_size = tile_size;

	map->borders[TILE_LEFT] = 0;
	map->borders[TILE_RIGHT] = 0;
	map->borders[TILE_TOP] = 0;
	map->borders[TILE_BOTTOM] = 0;

    map->tiles = malloc(rows * cols * sizeof(Tile));

	return map->tiles != NULL;
}

static vec2 TileGetPosition(const TileMap* map, size_t index)
{
	size_t col = index % map->cols;
	size_t row = (index / map->cols) + 1;

	return (vec2){ col * map->tile_size, (map->rows - row) * map->tile_size };
}

int TileMapLoadTiles(TileMap* map, TileID* tiles, TileType* types, size_t types_count)
{
	map->types = types;
	map->types_count = types_count;

	for (size_t index = 0; index < (map->rows * map->cols); ++index)
	{
		map->tiles[index].id = tiles[index];
		map->tiles[index].pos = TileGetPosition(map, index);
		map->tiles[index].type = TileMapGetType(map, tiles[index]);
	}

	return 1;
}

int TileMapStreamTiles(TileMap* map, void* stream, void*(*next)(void*, TileID*), TileType* types, size_t types_count)
{
	map->types = types;
	map->types_count = types_count;

	for (size_t index = 0; index < (map->rows * map->cols); ++index)
	{
		TileID id;
		stream = next(stream, &id);
		if (!stream) return 0;

		map->tiles[index].id = id;
		map->tiles[index].pos = TileGetPosition(map, index);
		map->tiles[index].type = TileMapGetType(map, id);
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
