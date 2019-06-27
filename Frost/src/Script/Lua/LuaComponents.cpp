#include "LuaComponents.h"


LuaTransformComponent::LuaTransformComponent(TransformComponent transform) : transform(transform) { }

void LuaTransformComponent::SetPosition(const glm::vec2& pos)
{
	transform.position = pos;
}

glm::vec2 LuaTransformComponent::GetPosition()
{
	return transform.position;
}

void LuaTransformComponent::SetDimension(const glm::vec2& dim)
{
	transform.dimension = dim;
}

glm::vec2 LuaTransformComponent::GetDimension()
{
	return transform.dimension;
}

LuaMovementComponent::LuaMovementComponent(MovementComponent movement) : movement(movement) { }

void LuaMovementComponent::SetSpeed(float speed)
{
	movement.movementSpeed = speed;
}

float LuaMovementComponent::GetSpeed()
{
	return movement.movementSpeed;
}

void LuaMovementComponent::SetJump(float jump)
{
	movement.jumpPower = jump;
}

float LuaMovementComponent::GetJump()
{
	return movement.jumpPower;
}

LuaPhysicsComponent::LuaPhysicsComponent(PhysicsComponent physics) : physics(physics) { }

void LuaPhysicsComponent::SetVelocity(const glm::vec2& vel)
{
	physics.body->setVelocity(vel);
}

glm::vec2 LuaPhysicsComponent::GetVelocity()
{
	return physics.body->getVelocity();
}

void LuaPhysicsComponent::Drop()
{
	physics.body->drop();
}

bool LuaPhysicsComponent::CollidesBottom()
{
	return physics.body->collidesBottom();
}

LuaCameraComponent::LuaCameraComponent(CameraComponent camera) : camera(camera) { }

void LuaCameraComponent::SetView(const glm::vec2& pos)
{
	Renderer::SetViewCenter(pos + camera.cameraOffset, camera.constraint);
}
