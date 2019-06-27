#pragma once

#include <entt/entt.hpp>

#include "Components.h"

#include "ScriptComponent.h"

#include "Input/Input.h"
#include "Utility/Timer.h"
#include "String/StringUtils.h"

struct ImageRenderSystem
{
	static void Tick(entt::registry& registry)
	{
		registry.view<ImageComponent, TransformComponent>().each([](auto entity, auto& img, auto& trans)
		{
			float x = trans.position.x - (img.image->getWidth() / 2.0f);
			float y = trans.position.y;
			img.image->renderF(x, y, img.frame, Renderer::GetViewMat(), "shader");
		});
	};
};

struct AnimationSystem
{
	static void PlayAnimation(const std::string& anim, AnimationComponent& animComp)
	{
		if (stringCompare(animComp.currentAnimation, anim))
			return;

		auto pos = animComp.animations.find(anim);

		if (pos != animComp.animations.end())
		{
			animComp.animations[anim].start();
			animComp.currentAnimation = anim;
		}
	}

	static void Tick(entt::registry& registry)
	{
		registry.view<MovementComponent, ImageComponent, AnimationComponent>().each([](auto entity, auto& move, auto& img, auto& anim)
		{
			if (!anim.currentAnimation.empty())
			{
				anim.animations[anim.currentAnimation].step(Timer::GetDeltaTime());
				img.frame = anim.animations[anim.currentAnimation].getFrame();
			}
		});
	}
};

struct TilePhysicsSystem
{
	static void Tick(entt::registry& registry)
	{
		registry.view<TransformComponent, PhysicsComponent>().each([](auto entity, auto& trans, auto& phys)
		{
			trans.position = phys.body->getPosition() - phys.bodyPos;
		});
	}
};

struct ScriptSystem
{
	static void Tick(entt::registry& registry, sol::state& lua)
	{
		auto view = registry.view<ScriptComponent>();

		for (auto entity : view) 
		{
			auto& script = view.get(entity);

			if (registry.has<TransformComponent>(entity))
				script.lua["Transform"] = LuaTransformComponent(registry.get<TransformComponent>(entity));

			if (registry.has<MovementComponent>(entity))
				script.lua["Movement"] = LuaMovementComponent(registry.get<MovementComponent>(entity));

			if (registry.has<PhysicsComponent>(entity))
				script.lua["Physics"] = LuaPhysicsComponent(registry.get<PhysicsComponent>(entity));

			if (registry.has<CameraComponent>(entity))
				script.lua["Camera"] = LuaCameraComponent(registry.get<CameraComponent>(entity));

			script.script(entity);
		}
	};
};

struct PlayerSystem
{
	static void Tick(entt::registry& registry)
	{
		registry.view<TransformComponent, MovementComponent, PhysicsComponent, ImageComponent, AnimationComponent, CameraComponent>().each([](auto entity, auto& trans, auto& move, auto& phys, auto& img, auto& anim, auto& cam)
		{
			glm::vec2 velocity = glm::vec2(0.0f, phys.body->getVelocity().y);

			if (Input::KeyPressed(KEY_A))
				velocity.x += -move.movementSpeed;
			if (Input::KeyPressed(KEY_D))
				velocity.x += move.movementSpeed;

			if (Input::KeyPressed(KEY_SPACE) && phys.body->collidesBottom())
				velocity.y = move.jumpPower;
			else if (Input::KeyReleased(KEY_SPACE))
				if (phys.body->getVelocity().y > 0)
					velocity.y = (phys.body->getVelocity().y * 0.5f);

			if (velocity.x < 0.0f)
				move.direction = MovementComponent::LEFT;
			else if (velocity.x > 0.0f)
				move.direction = MovementComponent::RIGHT;

			if (velocity.x == 0.0f)
				move.isMoving = false;
			else
				move.isMoving = true;

			if (velocity.y > 0.0f)
				move.isJumping = true;
			else
				move.isJumping = false;

			move.onFloor = phys.body->collidesBottom();
			phys.body->setVelocity(velocity);

			if (Input::KeyPressed(KEY_S))
				phys.body->drop();

			// animation
			if (move.direction == MovementComponent::LEFT)
				img.image->setRenderFlip(FLIP_HORIZONTAL);
			else
				img.image->setRenderFlip(FLIP_NONE);

			if (move.isJumping)
				AnimationSystem::PlayAnimation("jump", anim);
			else if (!(move.onFloor || move.isJumping))
				AnimationSystem::PlayAnimation("fall", anim);
			else if (move.isMoving)
				AnimationSystem::PlayAnimation("walk", anim);
			else
				AnimationSystem::PlayAnimation("idle", anim);

			Renderer::SetViewCenter(trans.position + cam.cameraOffset, cam.constraint);
		});
	};
};