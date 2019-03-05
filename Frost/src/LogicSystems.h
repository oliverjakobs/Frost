#pragma once

#include "RenderSystems.h"

class TestSystem : public BaseECSSystem
{
public:
	TestSystem() : BaseECSSystem()
	{
		addComponentType(PositionComponent::ID);
	}

	void tick(BaseECSComponent** components, float deltaTime) override
	{
		PositionComponent* posComp = (PositionComponent*)components[0];

		//DEBUG_MESSAGE("Position: " << posComp->position.x << ", " << posComp->position.y);
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

	void tick(BaseECSComponent** components, float deltaTime) override
	{
		PositionComponent* posComp = (PositionComponent*)components[0];
		PhysicsComponent* physComp = (PhysicsComponent*)components[1];

		posComp->position = physComp->body->getPosition() - physComp->bodyPos;
	}
};

class PlayerSystem : public BaseECSSystem
{
public:
	PlayerSystem() : BaseECSSystem()
	{
		addComponentType(PositionComponent::ID);
		addComponentType(MovementComponent::ID);
		addComponentType(PhysicsComponent::ID);
		addComponentType(ImageComponent::ID);
		addComponentType(AnimationComponent::ID);
		addComponentType(CameraComponent::ID, ECS_FLAG_OPTIONAL);
	}

	void tick(BaseECSComponent** components, float deltaTime) override
	{
		PositionComponent* posComp = (PositionComponent*)components[0];
		MovementComponent* moveComp = (MovementComponent*)components[1];
		PhysicsComponent* physComp = (PhysicsComponent*)components[2];
		ImageComponent* imgComp = (ImageComponent*)components[3];
		AnimationComponent* animComp = (AnimationComponent*)components[4];
		CameraComponent* camComp = (CameraComponent*)components[5];

		glm::vec2 velocity = glm::vec2(0.0f, physComp->body->getVelocity().y);

		if (Input::KeyDown(GLFW_KEY_A))
			velocity.x += -moveComp->movementSpeed;
		if (Input::KeyDown(GLFW_KEY_D))
			velocity.x += moveComp->movementSpeed;

		if (Input::KeyDown(GLFW_KEY_SPACE) && physComp->body->collidesBottom())
			velocity.y = moveComp->jumpPower;
		else if (Input::KeyReleased(GLFW_KEY_SPACE))
			if (physComp->body->getVelocity().y > 0)
				velocity.y = (physComp->body->getVelocity().y * 0.5f);

		if (velocity.x < 0.0f)
			moveComp->direction = MovementComponent::LEFT;
		else if (velocity.x > 0.0f)
			moveComp->direction = MovementComponent::RIGHT;

		if (velocity.x == 0.0f)
			moveComp->isMoving = false;
		else
			moveComp->isMoving = true;

		if (velocity.y > 0.0f)
			moveComp->isJumping = true;
		else
			moveComp->isJumping = false;

		moveComp->onFloor = physComp->body->collidesBottom();
		physComp->body->setVelocity(velocity);

		if (Input::KeyDown(GLFW_KEY_S))
			physComp->body->drop();

		// animation
		if (moveComp->direction == MovementComponent::LEFT)
			imgComp->image->setRenderFlip(FLIP_HORIZONTAL);
		else
			imgComp->image->setRenderFlip(FLIP_NONE);

		if (moveComp->isJumping)
			AnimationSystem::PlayAnimation("jump", animComp);
		else if (!(moveComp->onFloor || moveComp->isJumping))
			AnimationSystem::PlayAnimation("fall", animComp);
		else if (moveComp->isMoving)
			AnimationSystem::PlayAnimation("walk", animComp);
		else
			AnimationSystem::PlayAnimation("idle", animComp);

		if (camComp != nullptr)
			Renderer::SetViewCenter(posComp->position + camComp->cameraOffset, camComp->constraint);
	}
};