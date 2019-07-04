#pragma once

#include "Types.h"

#include "TileConfig.h"

enum TileType
{
	TILE_EMPTY = 0,
	TILE_SOLID = 1,
	TILE_PLATFORM = 2,
	TILE_SLOPE_LEFT = 3,
	TILE_SLOPE_RIGHT = 4,
	// not implemented yet
	TILE_CEIL_LEFT = 5,
	TILE_CEIL_RIGHT = 6
};

struct Tile
{
	uint id;
	glm::vec2 position;
	TileType type;
};


enum BodyType
{
	BODY_STATIC,		// does not move by any means 
	BODY_DYNAMIC		// can be moved, affected by forces
};

struct BodyDef
{
	float hW;
	float hH;
	BodyType type;
};

BodyType FromString(const std::string& str);