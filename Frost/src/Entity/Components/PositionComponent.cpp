#include "PositionComponent.hpp"

PositionComponent::PositionComponent(Entity* entity, const vec2 position)
	: Component(entity), m_position(position)
{
}

void PositionComponent::SetPosition(const vec2 position)
{
	m_position = position;
}

vec2 PositionComponent::GetPosition() const
{
	return m_position;
}