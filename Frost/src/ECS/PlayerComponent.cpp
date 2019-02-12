#include "PlayerComponent.h"

#include "Scrapbook/Utility.h"

PlayerComponent::PlayerComponent(float ms, float jp)
	: m_movementSpeed(ms), m_jumpPower(jp)
{
}

PlayerComponent::~PlayerComponent()
{
	m_physComp = nullptr;
	m_animComp = nullptr;
}

bool PlayerComponent::setEntity(Entity* entity)
{
	if (entity)
		entity->setFlag(FlagPlayer);

	m_physComp = getComponent<PhysicsComponent>(entity);

	if (m_physComp == nullptr)
	{
		DEBUG_MESSAGE("No Physics Component");
		return false;
	}

	m_animComp = getComponent<AnimationComponent>(entity);

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
	{
		m_animComp->play("jump");
	}
	else if (m_physComp->isFalling())
	{
		m_animComp->play("fall");
	}
	else
	{
		if (m_physComp->isMoving())
			m_animComp->play("walk");
		else
			m_animComp->play("idle");
	}

	Renderer::SetViewCenter(m_entity->getCenter().x, m_entity->getCenter().y, new Rect(glm::vec2(), m_physComp->getBody()->getMap()->getDimension() * m_physComp->getBody()->getMap()->getTileSize()));
	//Renderer::SetViewCenter(m_entity->getCenter().x, m_entity->getCenter().y, nullptr);
}

void PlayerComponent::onRender() const
{
}