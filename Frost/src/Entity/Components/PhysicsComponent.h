#pragma once

#include "PositionComponent.h"


class PhysicsComponent : public PositionComponent
{
private:
	glm::vec2 m_bodyPos;

public:
	PhysicsComponent(Entity* entity, const glm::vec2& bodyPos);
	virtual ~PhysicsComponent();

	void SetPosition(const glm::vec2& position) override;
	glm::vec2 GetPosition() const override;
};