#pragma once

#include "ecs/ecs.h"

#include "Animation.h"

#include "Core/Application.h"
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
	int frame;

	ImageComponent(Image* img) : image(img), frame(0) {}
};

struct AnimationComponent : public ECSComponent<AnimationComponent>
{
	std::map<std::string, Animation> animations;
	std::string currentAnimation;
	
	AnimationComponent(std::map<std::string, Animation> anims)
		: animations(anims), currentAnimation(std::string()) {}
};

struct CameraComponent : public ECSComponent<CameraComponent>
{
	Rect constraint;
	glm::vec2 cameraOffset;

	CameraComponent(const Rect& con, const glm::vec2& camOff) : constraint(con), cameraOffset(camOff) {}
};