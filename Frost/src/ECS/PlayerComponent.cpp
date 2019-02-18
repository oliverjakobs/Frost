#include "PlayerComponent.h"

#include "Scrapbook/Utility.h"

#include "Scene/Scene.h"

PlayerComponent::PlayerComponent(float ms, float jp)
	: m_movementSpeed(ms), m_jumpPower(jp)
{
}

PlayerComponent::~PlayerComponent()
{
	m_physComp = nullptr;
	m_animComp = nullptr;
}

PlayerComponent* PlayerComponent::clone()
{
	return new PlayerComponent(*this);
}

bool PlayerComponent::setEntity(Entity* entity)
{
	if (entity)
		entity->setFlag(FlagPlayer);

	m_physComp = entity->getComponent<PhysicsComponent>();

	if (m_physComp == nullptr)
	{
		DEBUG_MESSAGE("No Physics Component");
		return false;
	}

	m_animComp = entity->getComponent<AnimationComponent>();

	if (m_animComp == nullptr)
	{
		DEBUG_MESSAGE("No Animation Component");
		return false;
	}

	return (m_entity = entity) != nullptr;
}

void PlayerComponent::onInput()
{
	glm::vec2 velocity = glm::vec2(0.0f, m_physComp->getVelocity().y);

	if (Input::KeyDown(GLFW_KEY_A))
		velocity.x += -m_movementSpeed;
	if (Input::KeyDown(GLFW_KEY_D))
		velocity.x += m_movementSpeed;

	if (Input::KeyDown(GLFW_KEY_SPACE) && m_physComp->isOnFloor())
		velocity.y = m_jumpPower;
	else if (Input::KeyReleased(GLFW_KEY_SPACE))
		if (m_physComp->getVelocity().y > 0)
			velocity.y = (m_physComp->getVelocity().y * 0.5f);

	m_physComp->setVelocity(velocity);

	if (Input::KeyDown(GLFW_KEY_S))
		m_physComp->drop();
}

void PlayerComponent::onUpdate()
{
	if (m_physComp->getDirection() == LEFT)
		m_animComp->flip(FLIP_HORIZONTAL);
	else
		m_animComp->flip(FLIP_NONE);

	if (m_physComp->isJumping())
		m_animComp->play("jump");
	else if (m_physComp->isFalling())
		m_animComp->play("fall");
	else if (m_physComp->isMoving())
		m_animComp->play("walk");
	else
		m_animComp->play("idle");
	
	if (m_entity->getScene() != nullptr)
		Renderer::SetViewCenter(m_entity->getCenter().x, m_entity->getCenter().y, m_entity->getScene()->getConstraint());
}

void PlayerComponent::onRender() const
{
}