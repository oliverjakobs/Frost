#pragma once

#include <glm/glm.hpp>

class Body
{
private:
	glm::vec2 m_position;
	glm::vec2 m_halfDimension;

	glm::vec2 m_velocity;

public:
	Body(float x, float y, float hWidth, float hHeight);

	void Tick(float deltaTime, const glm::vec2& gravity);

	void ResolveCollision(const Body& b, const glm::vec2& oldPos);
	
	void SetPosition(const glm::vec2& position) { m_position = position; }
	const glm::vec2 GetPosition() const { return m_position; }

	void SetVelocity(const glm::vec2& velocity) { m_velocity = velocity; }
	const glm::vec2 GetVelocity() const { return m_velocity; }

	const glm::vec2 GetHalfDimension() const { return m_halfDimension; }
	const glm::vec2 GetDimension() const { return m_halfDimension * 2.0f; }

	// get the edges of the body
	const float GetX() const { return m_position.x - m_halfDimension.x; }
	const float GetX2() const { return m_position.x + m_halfDimension.x; }
	const float GetY() const { return m_position.y - m_halfDimension.y; }
	const float GetY2() const { return m_position.y + m_halfDimension.y; }
};