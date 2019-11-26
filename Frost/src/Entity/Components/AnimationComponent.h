#pragma once

#include "TextureComponent.h"

class Animation
{
private:
	int m_start;
	int m_length;

	float m_delay;
	float m_frameCounter;

	int m_frame;
public:
	Animation() = default;
	Animation(int start, int length, float delay);

	void Start();
	void Step(float deltaTime);
	int GetFrame();
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
	void OnRender(Scene* scene) override;
};