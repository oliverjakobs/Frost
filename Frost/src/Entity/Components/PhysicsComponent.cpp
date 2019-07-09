#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(Body* body, const glm::vec2& bodyPos) 
	: m_body(body), m_bodyPos(bodyPos) 
{

}

Body* PhysicsComponent::GetBody() const
{
	return m_body;
}

void PhysicsComponent::OnUpdate()
{
	m_body->SetPosition(m_entity->GetPosition() + m_bodyPos);

	m_body->Update();

	m_entity->SetPosition(m_body->GetPosition() - m_bodyPos);
}

void PhysicsComponent::OnRender()
{
}

void PhysicsComponent::OnRenderDebug()
{
	m_body->Render();
}
