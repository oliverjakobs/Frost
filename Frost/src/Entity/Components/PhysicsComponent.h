#pragma once

#include "Component.h"
#include "Tile/World.h"

using namespace tile;

class PhysicsComponent : public Component
{
private:
	std::shared_ptr<Body> m_body;
	glm::vec2 m_bodyPos;

public:
	PhysicsComponent(Entity* entity, std::shared_ptr<Body> body, const glm::vec2& bodyPos);

	std::shared_ptr<Body> GetBody() const;

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnRenderDebug() override;
};