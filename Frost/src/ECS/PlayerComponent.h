#pragma once

#include "PhysicsComponent.h"

class PlayerComponent : public Component
{
private:
	PhysicsComponent* m_physComp;

	float m_movementSpeed;
	float m_jumpPower;
public:
	PlayerComponent(float ms, float jp);
	~PlayerComponent();

	bool setEntity(Entity* e) override;

	void onInput() override;
	void onUpdate() override;
	void onRender() const override;
};