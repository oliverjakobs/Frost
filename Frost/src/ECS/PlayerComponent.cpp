#include "PlayerComponent.h"

#include "Scrapbook\Util\utils.h"
#include "Input.h"


PlayerComponent::PlayerComponent(float ms, float jp)
	: m_movementSpeed(ms), m_jumpPower(jp)
{
}

PlayerComponent::~PlayerComponent()
{
}

bool PlayerComponent::setEntity(Entity* e)
{
	m_physComp = getComponent<PhysicsComponent>(e);

	if (m_physComp == nullptr)
	{
		DEBUG_MESSAGE("No Physics Component");
		return false;
	}

	return (m_entity = e) != nullptr;
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
	

	//Renderer::SetViewPos(m_entity->getCenter().x, m_entity->getCenter().y, new Rect(glm::vec2(), m_physComp->getBody()->getMap()->getDimension() * m_physComp->getBody()->getMap()->getTileSize()));
}

void PlayerComponent::onRender() const
{
}