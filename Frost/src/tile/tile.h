#ifndef TILE_H
#define TILE_H

#include "math/math.h"

typedef enum
{
	TILE_LEFT,
	TILE_RIGHT,
	TILE_TOP,
	TILE_BOTTOM
} TileDirection;

typedef enum
{
	TILE_EMPTY = 0,
	TILE_SOLID,
	TILE_SLOPE_LEFT,
	TILE_SLOPE_RIGHT,
	TILE_PLATFORM,
	// not implemented yet
	TILE_CEIL_LEFT,
	TILE_CEIL_RIGHT
} TileType;

typedef enum
{
	TILE_BODY_STATIC,
	TILE_BODY_DYNAMIC
} TileBodyType;

typedef uint32_t TileID;

typedef struct
{
	TileID id;
	vec2 pos;
	TileType type;
} Tile;

typedef struct TileMap TileMap;
typedef struct TileBody TileBody;

#endif /* !TILE_H */
