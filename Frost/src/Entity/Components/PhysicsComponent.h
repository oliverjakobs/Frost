#pragma once

#include "Component.h"
#include "Tile/Tilemap.h"

class PhysicsComponent : public Component
{
private:
	Body* m_body;
	glm::vec2 m_bodyPos;

public:
	PhysicsComponent(Body* body, const glm::vec2& bodyPos);

	Body* GetBody() const;

	void OnUpdate() override;
	void OnRender() override;
	void OnRenderDebug() override;
};