#include "tile_map.h"

#include <stdlib.h>
#include <string.h>

int TileMapLoad(TileMap* map, size_t rows, size_t cols, float tile_size, tb_allocator* allocator)
{
	if (rows == 0 || cols == 0 || tile_size <= 0.0f) return 0;

    map->rows = rows;
    map->cols = cols;
    map->tile_size = tile_size;

	map->borders[TILE_LEFT] = 0;
	map->borders[TILE_RIGHT] = 0;
	map->borders[TILE_TOP] = 0;
	map->borders[TILE_BOTTOM] = 0;

	map->allocator = allocator;

	map->tiles = tb_mem_malloc(allocator, sizeof(Tile) * rows * cols);
	map->types = NULL;
	map->types_count = 0;

	return map->tiles != NULL;
}

static vec2 TileMapGetTilePos(const TileMap* map, size_t index)
{
	size_t col = index % map->cols;
	size_t row = (index / map->cols) + 1;

	return (vec2){ col * map->tile_size, (map->rows - row) * map->tile_size };
}

int TileMapLoadTiles(TileMap* map, TileID* tiles, TileType* types, size_t types_count)
{
	map->types = tb_mem_dup(map->allocator, types, sizeof(TileType) * types_count);
	if (!map->types) return 0;

	map->types_count = types_count;

	for (size_t index = 0; index < (map->rows * map->cols); ++index)
	{
		map->tiles[index].id = tiles[index];
		map->tiles[index].pos = TileMapGetTilePos(map, index);
		map->tiles[index].type = TileMapGetType(map, tiles[index]);
	}

	return 1;
}

void TileMapDestroy(TileMap* map)
{
	tb_mem_free(map->allocator, map->tiles);
	tb_mem_free(map->allocator, map->types);
}

int TileMapStreamTiles(TileMap* map, void* stream, void* (*next)(void*, TileID*), size_t len)
{
	if (len > (map->rows * map->cols)) return 0;

	for (size_t index = 0; index < len; ++index)
	{
		TileID id;
		stream = next(stream, &id);
		if (!stream) return 0;

		map->tiles[index].id = id;
		map->tiles[index].pos = TileMapGetTilePos(map, index);
		map->tiles[index].type = TileMapGetType(map, id);
	}
	return 1;
}

int TileMapStreamTypes(TileMap* map, void* stream, void* (*next)(void*, TileType*), size_t len)
{
	map->types = tb_mem_malloc(map->allocator, sizeof(TileType) * len);
	if (!map->types) return 0;

	for (size_t index = 0; index < len; ++index)
	{
		stream = next(stream, map->types + index);
		if (!stream) return 0;
	}

	map->types_count = len;
	return 1;
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
	return (id >= 0 || id < map->types_count) ? map->types[id] : TILE_EMPTY;
}

int32_t TileMapClamp(const TileMap* map, float x) { return (int32_t)floorf(x / map->tile_size); }

void TileMapSetBorder(TileMap* map, TileDirection dir, int8_t value) { map->borders[dir] = value; }
int8_t TileMapGetBorder(const TileMap* map, TileDirection dir) { return map->borders[dir]; }
