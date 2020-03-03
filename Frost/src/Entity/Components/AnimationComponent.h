#pragma once

#include "TextureComponent.h"

struct Anim
{
	int Start;
	int Length;

	float Delay;
	float FrameCounter;

	int Frame;

	Anim() = default;
	Anim(int start, int length, float delay);
};


class AnimationComponent : public Component
{
private:
	std::map<std::string, Anim> m_animations;
	std::string m_currentAnimation;

public:
	AnimationComponent(Entity* entity, const std::map<std::string, Anim>& anims);

	void PlayAnimation(const std::string& anim);
	std::string GetCurrent() const;

	void OnUpdate(Scene* scene, float deltaTime) override;
};