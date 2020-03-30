#pragma once

#include "PositionComponent.hpp"
#include "Physics/World.h"

class PhysicsComponent : public PositionComponent
{
private:
	std::shared_ptr<Body> m_body;

	vec2 m_bodyPos;

public:
	PhysicsComponent(Entity* entity, std::shared_ptr<Body> body, const vec2 bodyPos);
	virtual ~PhysicsComponent();

	void SetPosition(const vec2 position) override;
	vec2 GetPosition() const override;

	std::shared_ptr<Body> GetBody() const;
};