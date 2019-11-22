#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(Entity* entity, std::shared_ptr<Body> body, const glm::vec2& bodyPos)
	: Component(entity), m_body(body), m_bodyPos(bodyPos)
{
}

PhysicsComponent::~PhysicsComponent()
{
	m_body->GetWorld()->DestroyBody(m_body);
}

std::shared_ptr<Body> PhysicsComponent::GetBody() const
{
	return m_body;
}

void PhysicsComponent::OnUpdate(float deltaTime)
{
	m_entity->SetPosition(m_body->GetPosition() - m_bodyPos);
}

void PhysicsComponent::OnRender()
{
}

void PhysicsComponent::OnRenderDebug()
{

}
