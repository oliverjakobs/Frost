#pragma once

#include "ImageComponent.h"
#include "Animation.h"

class AnimationComponent : public Component
{
private:
	std::map<std::string, Animation> m_animations;
	std::string m_currentAnimation;

public:
	AnimationComponent(std::map<std::string, Animation> anims, const std::string& startingAnim) : m_animations(anims), m_currentAnimation(startingAnim) {}

	void PlayAnimation(const std::string& anim);
	std::string GetCurrent() const;

	void OnUpdate() override;
	void OnRender() override;
};