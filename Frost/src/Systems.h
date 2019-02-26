#pragma once

#include "Components.h"

class PlayerControlSystem : public BaseECSSystem
{
public:
	PlayerControlSystem() : BaseECSSystem()
	{
		addComponentType(PositionComponent::ID);
		addComponentType(MovementComponent::ID);
		addComponentType(PhysicsComponent::ID);
	}

	virtual void update(float deltaTime, BaseECSComponent** components) override
	{
		PositionComponent* posComp = ((PositionComponent*)components[0]);
		MovementComponent* moveComp = ((MovementComponent*)components[1]);
		PhysicsComponent* physComp = ((PhysicsComponent*)components[2]);

		glm::vec2 velocity = glm::vec2(0.0f, physComp->body->getVelocity().y);

		if (Input::KeyDown(GLFW_KEY_A))
			velocity.x += -moveComp->movementSpeed;
		if (Input::KeyDown(GLFW_KEY_D))
			velocity.x += moveComp->movementSpeed;

		if (Input::KeyDown(GLFW_KEY_SPACE) && physComp->onFloor)
			velocity.y = moveComp->jumpPower;
		else if (Input::KeyReleased(GLFW_KEY_SPACE))
			if (physComp->body->getVelocity().y > 0)
				velocity.y = (physComp->body->getVelocity().y * 0.5f);

		//moveComp->velocity = velocity;
		physComp->body->setVelocity(velocity);

		if (Input::KeyDown(GLFW_KEY_S))
			physComp->body->drop();

		/*if (m_physComp->getDirection() == LEFT)
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
		m_animComp->play("idle");*/

		//if (m_entity->getScene() != nullptr)
		//	Renderer::SetViewCenter(m_entity->getCenter().x, m_entity->getCenter().y, m_entity->getScene()->getConstraint());
	}
};

class TilePhysicsSystem : public BaseECSSystem
{
private:
	TileMap* m_map;

public:
	TilePhysicsSystem(TileMap* map) : BaseECSSystem(), m_map(map)
	{
		addComponentType(PositionComponent::ID);
		addComponentType(PhysicsComponent::ID);
	}

	virtual void update(float deltaTime, BaseECSComponent** components) override
	{
		PositionComponent* posComp = ((PositionComponent*)components[0]);
		PhysicsComponent* physComp = ((PhysicsComponent*)components[1]);

		posComp->position = physComp->body->getPosition() - physComp->bodyPos;
	}
};

class ImageRenderSystem : public BaseECSSystem
{
public:
	ImageRenderSystem() : BaseECSSystem()
	{
		addComponentType(PositionComponent::ID);
		addComponentType(ImageComponent::ID);
	}

	virtual void update(float deltaTime, BaseECSComponent** components) override
	{
		PositionComponent* posComp = (PositionComponent*)components[0];
		ImageComponent* imgComp = (ImageComponent*)components[1];

		float x = posComp->position.x - (imgComp->image->getWidth() / 2.0f);
		float y = posComp->position.y;

		imgComp->image->renderF(x, y, 0, Renderer::GetViewMat(), "shader");
	}
};