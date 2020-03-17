#include "World.hpp"

#include <algorithm>

World::World(const glm::vec2& gravity) : m_gravity(gravity)
{
}

void World::AddBody(std::shared_ptr<Body> body)
{
	body->SetWorld(this);
	m_bodies.push_back(body);
}

void World::RemoveBody(std::shared_ptr<Body> body)
{
	auto it = std::find_if(m_bodies.begin(), m_bodies.end(), [&](auto& e) { return e == body; });

	if (it != m_bodies.end())
	{
		(*it)->SetWorld(nullptr);
		m_bodies.erase(it);
	}
}

void World::Tick(float deltaTime)
{
	for (auto& body : m_bodies)
	{
		if (body->GetType() == BodyType::STATIC)
			continue;

		glm::vec2 oldPosition = body->GetPosition();

		body->Tick(deltaTime, m_gravity);

		for (auto& other : GetOtherBodies(body.get()))
		{
			body->ResolveCollision(*other, oldPosition);
		}
	}
}

std::vector<std::shared_ptr<Body>> World::GetOtherBodies(const Body* body) const
{
	std::vector<std::shared_ptr<Body>> others;

	for (auto& b : m_bodies)
	{
		if (b.get() != body)
			others.push_back(b);
	}

	return others;
}
