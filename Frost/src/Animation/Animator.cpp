#include "Animator.h"

void Animator::AddCondition(const std::string& name, std::function<bool(Entity*, int)> condition)
{
	m_conditions.insert({ name, condition });
}

void Animator::AddAnimation(const std::string& name, Animation animation)
{
	m_animations.insert({ name, animation });

	if (m_current.empty())
		m_current = name;
}

void Animator::Tick(float deltaTime)
{
	if (m_current.empty())
		return;

	m_animations.at(m_current).Tick(deltaTime);
}

void Animator::Play(const std::string& name)
{
	if (m_current.compare(name) == 0)
		return;

	if (m_animations.find(name) != m_animations.end())
	{
		m_animations[name].Start();
		m_current = name;
	}
}

int Animator::GetFrame() const
{
	if (m_current.empty())
		return 0;

	return m_animations.at(m_current).GetFrame();
}
