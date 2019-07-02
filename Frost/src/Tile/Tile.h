#pragma once

#include <glm/glm.hpp>

enum TileType
{
	Empty = 0,
	Solid = 1,
	Platform = 2,
	SlopeLeft = 3,
	SlopeRight = 4,
	// not implemented yet
	CeilLeft = 5,
	CeilRight = 6
};

struct Tile
{
	unsigned int id;
	glm::vec2 position;
	TileType type;
};