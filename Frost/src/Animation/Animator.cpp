#include "Animator.hpp"

#include "Entity/Entity.hpp"
#include "Entity/Components.hpp"

void Animator::LoadConditions()
{
	RegisterCondition("condition_jump", [](Entity* e, int s)
		{
			if (auto comp = EntityGetComponent<PhysicsComponent>(e))
				return comp->GetBody()->GetVelocity().y > 0.0f;

			return false;
		});
	RegisterCondition("condition_fall", [](Entity* e, int s)
		{
			if (auto comp = EntityGetComponent<PhysicsComponent>(e))
				return !comp->GetBody()->CollidesBottom() && comp->GetBody()->GetVelocity().y <= 0.0f;

			return false;
		});
	RegisterCondition("condition_walk", [](Entity* e, int s)
		{
			if (auto comp = EntityGetComponent<PhysicsComponent>(e))
				return comp->GetBody()->CollidesBottom() && comp->GetBody()->GetVelocity().x != 0.0f;

			return false;
		});
	RegisterCondition("condition_idle", [](Entity* e, int s)
		{
			if (auto comp = EntityGetComponent<PhysicsComponent>(e))
				return comp->GetBody()->CollidesBottom() && comp->GetBody()->GetVelocity().x == 0.0f;

			return false;
		});
}

void Animator::RegisterCondition(const std::string& name, std::function<bool(Entity*, int)> condition)
{
	m_conditions.insert({ name, condition });
}

void Animator::CreateAnimation(const std::string& name, int start, int length, float delay, std::vector<Transition> transitions)
{
	auto animation = std::make_shared<Animation>(start, length, delay);

	for (auto& transition : transitions)
	{
		animation->AddTransition(transition.first, transition.second);
	}

	m_animations.insert({ name, animation });

	if (m_current.empty())
		m_current = name;
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
