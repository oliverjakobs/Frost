#pragma once

#include "PhysicsComponent.h"
#include "AnimationComponent.h"

class PlayerComponent : public Component
{
private:
	PhysicsComponent* m_physComp;
	AnimationComponent* m_animComp;

	float m_movementSpeed;
	float m_jumpPower;
public:
	PlayerComponent(float ms, float jp);
	~PlayerComponent();

	PlayerComponent* clone();

	bool setEntity(Entity* e) override;

	void onInput() override;
	void onUpdate() override;
	void onRender() const override;
};