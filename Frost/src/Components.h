#pragma once

#include "ecs/ecs.h"

#include "TilePhysics/Tilemap.h"

struct PositionComponent : public ECSComponent<PositionComponent>
{
	glm::vec2 position;
};

struct MovementComponent : public ECSComponent<MovementComponent>
{
	enum Direction
	{
		LEFT,
		RIGHT
	} direction;

	glm::vec2 velocity;
	float movementSpeed;
	float jumpPower;
};

struct ImageComponent : public ECSComponent<ImageComponent>
{
	Image* image;
};

struct PhysicsComponent : public ECSComponent<PhysicsComponent>
{
	Body* body;
	glm::vec2 bodyPos;

	bool onFloor;
	bool jumping;
};
