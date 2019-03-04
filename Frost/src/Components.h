#pragma once

#include "ecs/ecs.h"

#include "Animation.h"

#include "TilePhysics/Tilemap.h"

struct PositionComponent : public ECSComponent<PositionComponent>
{
	glm::vec2 position;

	PositionComponent(const glm::vec2& pos) : position(pos) {}
};

struct MovementComponent : public ECSComponent<MovementComponent>
{
	enum Direction { RIGHT, LEFT } direction;

	glm::vec2 velocity;
	float movementSpeed;
	float jumpPower;

	bool onFloor;
	bool isMoving;
	bool isJumping;

	MovementComponent(float ms, float jp)
		: movementSpeed(ms), jumpPower(jp)
	{
		velocity = glm::vec2();
		direction = RIGHT;

		onFloor = false;
		isMoving = false;
		isJumping = false;
	}
};

struct PhysicsComponent : public ECSComponent<PhysicsComponent>
{
	Body* body;
	glm::vec2 bodyPos;

	PhysicsComponent(Body* b, const glm::vec2& bPos) : body(b), bodyPos(bPos) { }
};

struct ImageComponent : public ECSComponent<ImageComponent>
{
	Image* image;

	ImageComponent(Image* img) : image(img) {}
};

struct AnimationComponent : public ECSComponent<AnimationComponent>
{
	std::map<std::string, Animation> animations;
	std::string currentAnimation;

	Image* image;

	AnimationComponent(Image* img, std::map<std::string, Animation> anims)
		: image(img), animations(anims), currentAnimation(std::string()) {}
};