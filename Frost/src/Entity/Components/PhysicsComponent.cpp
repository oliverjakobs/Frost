#include "PhysicsComponent.hpp"

PhysicsComponent::PhysicsComponent(Entity* entity, std::shared_ptr<Body> body, const glm::vec2& bodyPos)
	: PositionComponent(entity, glm::vec2()), m_body(body), m_bodyPos(bodyPos)
{
}

PhysicsComponent::~PhysicsComponent()
{
	if (m_body->world != nullptr)
		WorldRemoveBody(m_body->world, m_body.get());
}

void PhysicsComponent::SetPosition(const glm::vec2& position)
{
	m_body->position = position + m_bodyPos;
}

glm::vec2 PhysicsComponent::GetPosition() const
{
	return m_body->position - m_bodyPos;
}

std::shared_ptr<Body> PhysicsComponent::GetBody() const
{
	return m_body;
}