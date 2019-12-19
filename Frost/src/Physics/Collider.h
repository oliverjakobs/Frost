#pragma once

#include "Body.h"

struct Collider
{
	static bool CheckCollision(const Body& a, const Body& b);
};