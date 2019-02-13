#pragma once

#include "Component.h"
#include "TilePhysics/Tilemap.h"

enum Direction
{
	LEFT,
	RIGHT
};

class PhysicsComponent : public Component
{
private:
	Body* m_body;
	BodyDef m_bodyDef;

	bool m_isMoving;
	bool m_isJumping;

	glm::vec2 m_bodyPos;

	Direction m_direction;
public:
	PhysicsComponent(BodyDef bodyDef);
	PhysicsComponent(BodyDef bodyDef, const glm::vec2& bodyPos);
	~PhysicsComponent();

	void load() override;
	void unload() override;

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