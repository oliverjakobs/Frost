#pragma once

#include "Components.h"

class TestSystem : public BaseECSSystem
{
public:
	TestSystem() : BaseECSSystem()
	{
		addComponentType(PositionComponent::ID);
	}

	virtual void update(float deltaTime, BaseECSComponent** components) override
	{
		PositionComponent* posComp = (PositionComponent*)components[0];

		//DEBUG_MESSAGE("Position: " << posComp->position.x << ", " << posComp->position.y);
	}
};

class PlayerControlSystem : public BaseECSSystem
{
public:
	PlayerControlSystem() : BaseECSSystem()
	{
		addComponentType(PositionComponent::ID);
		addComponentType(MovementComponent::ID);
		addComponentType(PhysicsComponent::ID);
		addComponentType(AnimationComponent::ID, ECS_FLAG_OPTIONAL);
	}

	virtual void update(float deltaTime, BaseECSComponent** components) override
	{
		PositionComponent* posComp = (PositionComponent*)components[0];
		MovementComponent* moveComp = (MovementComponent*)components[1];
		PhysicsComponent* physComp = (PhysicsComponent*)components[2];
		AnimationComponent* animComp = (AnimationComponent*)components[3];

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
		PositionComponent* posComp = (PositionComponent*)components[0];
		PhysicsComponent* physComp = (PhysicsComponent*)components[1];

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

	virtual void render(BaseECSComponent** components) override
	{
		PositionComponent* posComp = (PositionComponent*)components[0];
		ImageComponent* imgComp = (ImageComponent*)components[1];

		float x = posComp->position.x - (imgComp->image->getWidth() / 2.0f);
		float y = posComp->position.y;

		imgComp->image->renderF(x, y, 0, Renderer::GetViewMat(), "shader");
	}
};

class AnimationSystem : public BaseECSSystem
{
public:
	AnimationSystem() : BaseECSSystem()
	{
		addComponentType(PositionComponent::ID);
		addComponentType(MovementComponent::ID);
		addComponentType(AnimationComponent::ID);
	}

	void playAnimation(const std::string& anim, AnimationComponent* animComp)
	{
		if (stringCompare(animComp->currentAnimation, anim))
			return;

		auto pos = animComp->animations.find(anim);

		if (pos != animComp->animations.end())
		{
			animComp->animations[anim].start();
			animComp->currentAnimation = anim;
		}
	}

	virtual void update(float deltaTime, BaseECSComponent** components) override
	{
		MovementComponent* moveComp = (MovementComponent*)components[1];
		AnimationComponent* animComp = (AnimationComponent*)components[2];

		if (moveComp->direction == MovementComponent::LEFT)
			animComp->image->setRenderFlip(FLIP_HORIZONTAL);
		else
			animComp->image->setRenderFlip(FLIP_NONE);

		if (moveComp->isJumping)
			playAnimation("jump", animComp);
		else if (!(moveComp->onFloor || moveComp->isJumping))
			playAnimation("fall", animComp);
		else if (moveComp->isMoving)
			playAnimation("walk", animComp);
		else
			playAnimation("idle", animComp);

		if (!animComp->currentAnimation.empty())
			animComp->animations[animComp->currentAnimation].step(deltaTime);
	}

	virtual void render(BaseECSComponent** components) override
	{
		PositionComponent* posComp = (PositionComponent*)components[0];
		AnimationComponent* animComp = (AnimationComponent*)components[2];

		float x = posComp->position.x - (animComp->image->getWidth() / 2.0f);
		float y = posComp->position.y;

		if (!animComp->currentAnimation.empty())
			animComp->image->renderF(x, y, animComp->animations[animComp->currentAnimation].getFrame(), Renderer::GetViewMat(), "shader");
	}
};