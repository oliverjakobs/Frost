#pragma once

#include "ecs/ecs.h"

#include "Animation.h"

#include "TilePhysics/Tilemap.h"

struct PositionComponent : public ECSComponent<PositionComponent>
{
	glm::vec2 position;
};

struct MovementComponent : public ECSComponent<MovementComponent>
{
	enum Direction { LEFT, RIGHT } direction;

	glm::vec2 velocity;
	float movementSpeed;
	float jumpPower;

	bool onFloor;
	bool isMoving;
	bool isJumping;
};

struct PhysicsComponent : public ECSComponent<PhysicsComponent>
{
	Body* body;
	glm::vec2 bodyPos;
};

struct ImageComponent : public ECSComponent<ImageComponent>
{
	Image* image;
};

struct AnimationComponent : public ECSComponent<AnimationComponent>
{
	std::map<std::string, Animation> animations;
	std::string currentAnimation;

	Image* image;
};