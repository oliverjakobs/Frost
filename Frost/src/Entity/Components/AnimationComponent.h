#pragma once

#include "TextureComponent.h"

struct Animation
{
	int Start;
	int Length;

	float Delay;
	float FrameCounter;

	int Frame;

	Animation() = default;
	Animation(int start, int length, float delay);
};

class AnimationComponent : public Component
{
private:
	std::map<std::string, Animation> m_animations;
	std::string m_currentAnimation;

public:
	AnimationComponent(Entity* entity, const std::map<std::string, Animation>& anims);

	void PlayAnimation(const std::string& anim);
	std::string GetCurrent() const;

	void OnUpdate(Scene* scene, float deltaTime) override;
};