#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(Entity* entity, std::shared_ptr<Body> body, const glm::vec2& bodyPos)
	: Component(entity), m_body(body), m_bodyPos(bodyPos)
{

}

std::shared_ptr<Body> PhysicsComponent::GetBody() const
{
	return m_body;
}

void PhysicsComponent::OnUpdate(float deltaTime)
{
	m_body->SetPosition(m_entity->GetPosition() + m_bodyPos);

	m_body->Tick(deltaTime);

	m_entity->SetPosition(m_body->GetPosition() - m_bodyPos);
}

void PhysicsComponent::OnRender()
{
}

void PhysicsComponent::OnRenderDebug()
{
	//m_body->Render();
}
