#pragma once

#include "TextureComponent.hpp"
#include "Animation/Animator.h"

class AnimationComponent : public Component
{
private:
	std::shared_ptr<Animator> m_animator;

public:
	AnimationComponent(Entity* entity, std::shared_ptr<Animator> animator);

	void OnUpdate(Scene* scene, float deltaTime) override;
};