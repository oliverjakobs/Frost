#pragma once

#include "TextureComponent.h"
#include "Animation.h"

class AnimationComponent : public Component
{
private:
	std::map<std::string, Animation> m_animations;
	std::string m_currentAnimation;

public:
	AnimationComponent(Entity* entity, std::map<std::string, Animation> anims);

	void PlayAnimation(const std::string& anim);
	std::string GetCurrent() const;

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
};