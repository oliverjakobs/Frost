#pragma once

#include "Animation.h"

#include <unordered_map>
#include <functional>

class Animator
{
private:
	std::unordered_map<std::string, std::function<bool(Entity*, int)>> m_conditions;
	std::unordered_map<std::string, std::shared_ptr<Animation>> m_animations;
	std::string m_current;

public:

	void RegisterCondition(const std::string& name, std::function<bool(Entity*, int)> condition);
	std::shared_ptr<Animation> CreateAnimation(const std::string& name, int start, int length, float delay, std::initializer_list<Transition> transitions);

	void Tick(Entity* entity, float deltaTime);
	void Play(const std::string& name);

	int GetFrame() const;
};