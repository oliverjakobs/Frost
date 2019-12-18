#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(Entity* entity, std::shared_ptr<Body> body, const glm::vec2& bodyPos)
	: PositionComponent(entity, glm::vec2()), m_body(body), m_bodyPos(bodyPos)
{
}

PhysicsComponent::~PhysicsComponent()
{
	if (m_body->GetWorld() != nullptr)
		m_body->GetWorld()->RemoveBody(m_body);
}

void PhysicsComponent::SetPosition(const glm::vec2& position)
{
	m_body->SetPosition(position + m_bodyPos);
}

glm::vec2 PhysicsComponent::GetPosition() const
{
	return m_body->GetPosition() - m_bodyPos;
}

std::shared_ptr<Body> PhysicsComponent::GetBody() const
{
	return m_body;
}