#pragma once

#include <glm/glm.hpp>

class World;

enum class BodyType
{
	BODY_TYPE_STATIC,
	BODY_TYPE_DYNAMIC
};

class Body
{
private:
	glm::vec2 m_position;
	glm::vec2 m_halfDimension;

	glm::vec2 m_velocity;

	BodyType m_type;

	// CollisionState
	bool m_collidesBottom;
	bool m_collidesTop;
	bool m_collidesLeft;
	bool m_collidesRight;

	World* m_world;
public:
	Body(float x, float y, float hWidth, float hHeight, BodyType type);

	void SetWorld(World* world) { m_world = world; }
	World* GetWorld() const { return m_world; }

	void Tick(float deltaTime, const glm::vec2& gravity);

	void ResolveCollision(const Body& b, const glm::vec2& oldPos);
	
	void SetPosition(const glm::vec2& position) { m_position = position; }
	const glm::vec2 GetPosition() const { return m_position; }

	void SetVelocity(const glm::vec2& velocity) { m_velocity = velocity; }
	const glm::vec2 GetVelocity() const { return m_velocity; }

	const glm::vec2 GetHalfDimension() const { return m_halfDimension; }
	const glm::vec2 GetDimension() const { return m_halfDimension * 2.0f; }

	BodyType GetType() const { return m_type; }

	// get the collision state
	const bool CollidesBottom() const { return m_collidesBottom; }
	const bool CollidesTop() const { return m_collidesTop; }
	const bool CollidesLeft() const { return m_collidesLeft; }
	const bool CollidesRight() const { return m_collidesRight; }

	// get the edges of the body
	const float GetX() const { return m_position.x - m_halfDimension.x; }
	const float GetX2() const { return m_position.x + m_halfDimension.x; }
	const float GetY() const { return m_position.y - m_halfDimension.y; }
	const float GetY2() const { return m_position.y + m_halfDimension.y; }
};