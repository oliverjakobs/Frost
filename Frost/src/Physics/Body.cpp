#include "Body.h"

#include <algorithm>

Body::Body(float x, float y, float hWidth, float hHeight, BodyType type)
	: m_position(x, y), m_halfDimension(hWidth, hHeight), m_velocity(0.0f, 0.0f), m_type(type), m_world(nullptr)
{
}

void Body::Tick(float deltaTime, const glm::vec2& gravity)
{
	m_velocity += gravity * deltaTime;

	m_collidesBottom = false;
	m_collidesTop = false;
	m_collidesLeft = false;
	m_collidesRight = false;

	m_position += m_velocity * deltaTime;
}

void Body::ResolveCollision(const Body& b, const glm::vec2& oldPos)
{
	glm::vec2 overlap;

	overlap.x = std::min(GetX2() - b.GetX(), b.GetX2() - GetX());
	overlap.y = std::min((oldPos.y + m_halfDimension.y) - b.GetY(), b.GetY2() - (oldPos.y - m_halfDimension.y));

	// horizontal resolve
	if (overlap.x > 0.0f && overlap.y > 0.0f)
	{
		if (m_velocity.x < 0.0f)
		{
			m_position.x += std::max(overlap.x, 0.0f);
			m_velocity.x = 0.0f;
			m_collidesLeft = true;
		}
		else if (m_velocity.x > 0.0f)
		{
			m_position.x -= std::max(overlap.x, 0.0f);
			m_velocity.x = 0.0f;
			m_collidesRight = true;
		}
	}

	overlap.x = std::min((oldPos.x + m_halfDimension.x) - b.GetX(), b.GetX2() - (oldPos.x - m_halfDimension.x));
	overlap.y = std::min(GetY2() - b.GetY(), b.GetY2() - GetY());

	// vertical resolve
	if (overlap.x > 0.0f && overlap.y > 0.0f)
	{
		if (m_velocity.y < 0.0f)
		{
			m_position.y += std::max(overlap.y, 0.0f);
			m_velocity.y = 0.0f;
			m_collidesBottom = true;
		}
		else if (m_velocity.y > 0.0f)
		{
			m_position.y -= std::max(overlap.y, 0.0f);
			m_velocity.y = 0.0f;
			m_collidesTop = true;
		}
	}
}
