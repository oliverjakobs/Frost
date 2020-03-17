#include "Color.hpp"

color BlendColor(const color& c, float a)
{
	return color(c.r, c.g, c.b, a);
}