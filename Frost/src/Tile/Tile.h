#pragma once

#include "Maths/Maths.h"
#include "String/StringUtils.h"

#include "TileConfig.h"

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


enum BodyType
{
	BodyTypeStatic,		// does not move by any means 
	BodyTypeDynamic		// can be moved, affected by forces
};

struct BodyDef
{
	float hW;
	float hH;
	BodyType type;
};

BodyType FromString(const std::string& str);