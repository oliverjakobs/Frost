#pragma once

#include <glm/glm.hpp>
#include <string>

#include "ECS/Components.h"

struct LuaTransformComponent
{
private:
	TransformComponent transform;

public:
	LuaTransformComponent(TransformComponent transform);

	void SetPosition(const glm::vec2& pos);
	glm::vec2 GetPosition();

	void SetDimension(const glm::vec2& dim);
	glm::vec2 GetDimension();
};

struct LuaMovementComponent
{
private:
	MovementComponent movement;

public:
	LuaMovementComponent(MovementComponent movement);

	void SetSpeed(float speed);
	float GetSpeed();

	void SetJump(float jump);
	float GetJump();
};

struct LuaPhysicsComponent
{
private:
	PhysicsComponent physics;

public:
	LuaPhysicsComponent(PhysicsComponent physics);

	void SetVelocity(const glm::vec2& vel);
	glm::vec2 GetVelocity();

	void Drop();
	bool CollidesBottom();
};

struct LuaCameraComponent
{
private:
	CameraComponent camera;

public:
	LuaCameraComponent(CameraComponent camera);

	void SetView(const glm::vec2& pos);
};