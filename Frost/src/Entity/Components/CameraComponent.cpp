#include "CameraComponent.h"

CameraComponent::CameraComponent(const Rect& constraint, const glm::vec2& offset) 
	: m_constraint(constraint), m_offset(offset)
{

}

Rect CameraComponent::GetConstraint() const
{
	return m_constraint;
}

glm::vec2 CameraComponent::GetOffset() const
{
	return m_offset;
}

void CameraComponent::OnUpdate()
{
}

void CameraComponent::OnRender()
{
}
