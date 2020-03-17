#include "PositionComponent.hpp"

PositionComponent::PositionComponent(Entity* entity, const glm::vec2& position)
	: Component(entity), m_position(position)
{
}

void PositionComponent::SetPosition(const glm::vec2& position)
{
	m_position = position;
}

glm::vec2 PositionComponent::GetPosition() const
{
	return m_position;
}