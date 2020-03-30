#include "PhysicsComponent.hpp"

PhysicsComponent::PhysicsComponent(Entity* entity, std::shared_ptr<Body> body, const vec2 bodyPos)
	: PositionComponent(entity, vec2_(0.0f, 0.0f)), m_body(body), m_bodyPos(bodyPos)
{
}

PhysicsComponent::~PhysicsComponent()
{
	if (m_body->world != nullptr)
		WorldRemoveBody(m_body->world, m_body.get());
}

void PhysicsComponent::SetPosition(const vec2 position)
{
	m_body->position = vec2_add(position, m_bodyPos);
}

vec2 PhysicsComponent::GetPosition() const
{
	return vec2_sub(m_body->position, m_bodyPos);
}

std::shared_ptr<Body> PhysicsComponent::GetBody() const
{
	return m_body;
}