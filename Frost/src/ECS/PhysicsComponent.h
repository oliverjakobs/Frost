#pragma once

#include "Component.h"
#include "TilePhysics/Body.h"

enum Direction
{
	LEFT,
	RIGHT
};

class PhysicsComponent : public Component
{
private:
	Body* m_body;

	bool m_isMoving;
	bool m_isJumping;

	glm::vec2 m_bodyPos;

	Direction m_direction;
public:
	PhysicsComponent(Body* body);
	PhysicsComponent(Body* body, const glm::vec2& bodyPos);
	~PhysicsComponent();

	void onInput() override;
	void onUpdate() override;
	void onRender() const override;
	void onRenderDebug() const override;

	void setVelocity(const glm::vec2& vel);
	glm::vec2 getVelocity() const;

	void drop();

	Direction getDirection() const;
	Body* getBody() const;

	bool isOnFloor() const;

	bool isJumping() const;
	bool isFalling() const;
	bool isMoving() const;
};