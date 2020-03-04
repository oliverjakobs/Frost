#include "Animator.h"

void Animator::RegisterCondition(const std::string& name, std::function<bool(Entity*, int)> condition)
{
	m_conditions.insert({ name, condition });
}

std::shared_ptr<Animation> Animator::CreateAnimation(const std::string& name, int start, int length, float delay, std::initializer_list<Transition> transitions)
{
	auto animation = std::make_shared<Animation>(start, length, delay);

	for (auto& transition : transitions)
	{
		animation->AddTransition(transition.first, transition.second);
	}

	m_animations.insert({ name, animation });

	if (m_current.empty())
		m_current = name;

	return animation;
}

void Animator::Tick(Entity* entity, float deltaTime)
{
	if (m_current.empty())
		return;

	auto& animation = m_animations.at(m_current);

	animation->Tick(deltaTime);
	for (const auto& [name, next] : animation->GetTransitions())
	{
		if (m_conditions.find(name) != m_conditions.end())
		{
			if (m_conditions.at(name)(entity, 0))
				Play(next);
		}
	}
}

void Animator::Play(const std::string& name)
{
	if (m_current.compare(name) == 0)
		return;

	if (m_animations.find(name) != m_animations.end())
	{
		m_animations[name]->Start();
		m_current = name;
	}
}

int Animator::GetFrame() const
{
	if (m_current.empty())
		return 0;

	return m_animations.at(m_current)->GetFrame();
}
