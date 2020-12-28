#ifndef TILE_H
#define TILE_H

#include "math/math.h"

typedef enum
{
	TILE_EMPTY = 0,
	TILE_SOLID = 1,
	TILE_SLOPE_LEFT = 2,
	TILE_SLOPE_RIGHT = 3,
	TILE_PLATFORM = 4,
	// not implemented yet
	TILE_CEIL_LEFT = 5,
	TILE_CEIL_RIGHT = 6
} TileType;

typedef uint32_t TileID;

typedef struct
{
	TileID id;
	vec2 pos;
	TileType type;
} Tile;

#endif /* !TILE_H */
