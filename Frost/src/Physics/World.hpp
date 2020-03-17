#pragma once

#include "Body.hpp"

#include <vector>
#include <memory>

class World
{
private:
	std::vector<std::shared_ptr<Body>> m_bodies;

	glm::vec2 m_gravity;

public:
	World(const glm::vec2& gravity);

	void AddBody(std::shared_ptr<Body> body);
	void RemoveBody(std::shared_ptr<Body> body);

	void Tick(float deltaTime);

	const std::vector<std::shared_ptr<Body>>& GetBodies() const { return m_bodies; }
	std::vector<std::shared_ptr<Body>> GetOtherBodies(const Body* body) const;
};