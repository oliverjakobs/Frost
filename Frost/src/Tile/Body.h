#pragma once

#include "Tile.h"

namespace tile
{
	class World;

	class Body
	{
	private:
		glm::vec2 m_position;
		glm::vec2 m_halfDimension;

		glm::vec2 m_velocity;

		// -----------------------------------
		float m_sensorOffset;			// offset to the edges
		glm::vec2 m_offsetHorizontal;
		glm::vec2 m_offsetVertical;

		// CollisionState
		bool m_collidesBottom;
		bool m_collidesTop;
		bool m_collidesLeft;
		bool m_collidesRight;

		// Slopes
		bool m_onSlope;
		bool m_slopeDetected;

		float m_gravityScale;	// amplifies the magnitude of the gravity
		bool m_drop;			// should the body drop through a platform

		World* m_world;
		BodyType m_type;

		// accessible through world
		Body(World* world, const glm::vec2& pos, const glm::vec2& halfDim, BodyType type);

		// move in x and y direction seperatly
		void MoveX(float x);
		void MoveY(float y);

		// world collision detection
		bool CheckBottom(const glm::vec2& position, const glm::vec2& oldPosition, float* groundY);
		bool CheckTop(const glm::vec2& position, const glm::vec2& oldPosition, float* groundY);
		bool CheckLeft(const glm::vec2& position, const glm::vec2& oldPosition, float* wallX);
		bool CheckRight(const glm::vec2& position, const glm::vec2& oldPosition, float* wallX);

		// body collision
		void ResolveBodyCollision(const Body& b, const glm::vec2& oldPos);

		// get sensors around the position
		Line GetSensorBottom(const glm::vec2& position, const glm::vec2& offset) const;
		Line GetSensorTop(const glm::vec2& position, const glm::vec2& offset) const;
		Line GetSensorLeft(const glm::vec2& position, const glm::vec2& offset) const;
		Line GetSensorRight(const glm::vec2& position, const glm::vec2& offset) const;
	public:
		void Tick(float deltaTime);

		void SetPosition(const glm::vec2& pos) { m_position = pos; }
		void SetVelocity(const glm::vec2& vel) { m_velocity = vel; }

		void Drop() { m_drop = true; }

		const glm::vec2 GetPosition() const { return m_position; }
		const glm::vec2 GetVelocity() const { return m_velocity; }

		const float GetHalfWidth() const { return m_halfDimension.x; }
		const float GetHalfHeight() const { return m_halfDimension.y; }
		const glm::vec2 GetHalfDimension() const { return m_halfDimension; }

		const float GetWidth() const { return m_halfDimension.x * 2.0f; }
		const float GetHeight() const { return m_halfDimension.y * 2.0f; }
		const glm::vec2 GetDimension() const { return m_halfDimension * 2.0f; }

		// get the edges of the body
		const float GetX() const { return m_position.x - m_halfDimension.x; }
		const float GetX2() const { return m_position.x + m_halfDimension.x; }
		const float GetY() const { return m_position.y - m_halfDimension.y; }
		const float GetY2() const { return m_position.y + m_halfDimension.y; }

		// get the collision state
		const bool CollidesBottom() const { return m_collidesBottom; }
		const bool CollidesTop() const { return m_collidesTop; }
		const bool CollidesLeft() const { return m_collidesLeft; }
		const bool CollidesRight() const { return m_collidesRight; }

		const BodyType GetType() const { return m_type; }
		World* GetWorld() const { return m_world; }

		friend World;
	};
}