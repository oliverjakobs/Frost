#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(Entity* entity, const glm::vec2& bodyPos)
	: PositionComponent(entity, glm::vec2()), m_bodyPos(bodyPos)
{
}

PhysicsComponent::~PhysicsComponent()
{

}

void PhysicsComponent::SetPosition(const glm::vec2& position)
{

}

glm::vec2 PhysicsComponent::GetPosition() const
{
	return glm::vec2();
}