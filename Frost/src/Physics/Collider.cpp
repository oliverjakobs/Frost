#include "Collider.h"

#include <algorithm>

bool Collider::CheckCollision(const Body& a, const Body& b)
{
	return a.GetX() < b.GetX2() && a.GetX2() > b.GetX() && a.GetY() < b.GetY2() && a.GetY2() > b.GetY();
}
