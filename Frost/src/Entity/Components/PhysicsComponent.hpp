#pragma once

#include "PositionComponent.hpp"
#include "Physics/World.h"

class PhysicsComponent : public PositionComponent
{
private:
	std::shared_ptr<Body> m_body;

	glm::vec2 m_bodyPos;

public:
	PhysicsComponent(Entity* entity, std::shared_ptr<Body> body, const glm::vec2& bodyPos);
	virtual ~PhysicsComponent();

	void SetPosition(const glm::vec2& position) override;
	glm::vec2 GetPosition() const override;

	std::shared_ptr<Body> GetBody() const;
};