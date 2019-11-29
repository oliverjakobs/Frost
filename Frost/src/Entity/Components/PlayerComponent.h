#pragma once

#include "Component.h"

class PlayerComponent : public Component
{
private:
	float m_movementSpeed;
	float m_jumpPower;

public:
	PlayerComponent(Entity* entity, float ms, float jp);

	void OnUpdate(Scene* scene, float deltaTime) override;
	void OnRender(Scene* scene) override;
};
