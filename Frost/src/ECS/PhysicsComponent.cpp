#include "PhysicsComponent.h"

#include "Scene/Scene.h"

PhysicsComponent::PhysicsComponent(BodyDef bodyDef)
	: m_bodyDef(bodyDef), m_direction(RIGHT)
{
	m_bodyPos = glm::vec2(0.0f, bodyDef.hW);

	m_isMoving = false;
	m_isJumping = false;

	m_body = nullptr;
}

PhysicsComponent::PhysicsComponent(BodyDef bodyDef, const glm::vec2& bodyPos)
	: m_bodyDef(bodyDef), m_bodyPos(bodyPos), m_direction(RIGHT)
{
	m_isMoving = false;
	m_isJumping = false;

	m_body = nullptr;
}

PhysicsComponent::~PhysicsComponent()
{
	if (m_body != nullptr)
	{
		m_body->getMap()->destroyBody(m_body);
		m_body = nullptr;
	}
}

void PhysicsComponent::load()
{
	if (m_entity->getScene() != nullptr)
	{
		glm::vec2 pos = m_entity->getPosition() + m_bodyPos;
		m_body = m_entity->getScene()->getMap()->createBody(pos.x, pos.y, m_bodyDef.hW, m_bodyDef.hH, m_bodyDef.type);
	}
}

void PhysicsComponent::unload()
{
	if (m_body != nullptr)
	{
		m_body->getMap()->destroyBody(m_body);
		m_body = nullptr;
	}
}

void PhysicsComponent::onInput()
{
}

void PhysicsComponent::onUpdate()
{
	m_entity->setPosition(m_body->getPosition() - m_bodyPos);
}

void PhysicsComponent::onRender() const
{

}

void PhysicsComponent::onRenderDebug() const
{
	m_body->onRender();
}

void PhysicsComponent::setVelocity(const glm::vec2& vel)
{
	if (vel.x < 0.0f)
		m_direction = LEFT;
	else if (vel.x > 0.0f)
		m_direction = RIGHT;

	if (vel.x == 0.0f)
		m_isMoving = false;
	else
		m_isMoving = true;

	if (vel.y > 0.0f)
		m_isJumping = true;
	else
		m_isJumping = false;

	m_body->setVelocity(vel);
}

glm::vec2 PhysicsComponent::getVelocity() const
{
	return m_body->getVelocity();
}

void PhysicsComponent::drop()
{
	m_body->drop();
}

Direction PhysicsComponent::getDirection() const
{
	return m_direction;
}

Body* PhysicsComponent::getBody() const
{
	return m_body;
}

bool PhysicsComponent::isOnFloor() const
{
	return m_body->collidesBottom();
}

bool PhysicsComponent::isJumping() const
{
	return m_isJumping;
}

bool PhysicsComponent::isFalling() const
{
	return !(isOnFloor() || isJumping());
}

bool PhysicsComponent::isMoving() const
{
	return m_isMoving;
}