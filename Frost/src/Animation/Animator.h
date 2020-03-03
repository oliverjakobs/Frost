#pragma once

#include "Animation.h"

class Animator
{
private:
	std::map<std::string, std::function<bool(Entity*, int)>> m_conditions;

	std::map<std::string, Animation> m_animations;
	std::string m_current;

public:

	void AddCondition(const std::string& name, std::function<bool(Entity*, int)> condition);
	void AddAnimation(const std::string& name, Animation animation);

	void Tick(float deltaTime);
	void Play(const std::string& name);

	int GetFrame() const;
};