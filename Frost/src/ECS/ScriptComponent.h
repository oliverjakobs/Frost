#pragma once

#include "Components.h"
#include "Script/Lua/LuaBinding.h"

struct ScriptComponent
{
	sol::state lua;
	sol::function script;

	ScriptComponent(const std::string& path, const std::string& funtionName) 
	{
		lua.open_libraries(sol::lib::base);

		lua.new_usertype<glm::vec2>("Vec2",
			sol::constructors<glm::vec2(), glm::vec2(float, float)>(),
			"x", &glm::vec2::x,
			"y", &glm::vec2::y
			);

		// Input
		lua.new_usertype<LuaInput>("Input",
			sol::constructors<LuaInput()>(),
			"KeyPressed", &LuaInput::KeyPressed,
			"KeyReleased", &LuaInput::KeyReleased
			);

		// Components
		lua.new_usertype<LuaTransformComponent>("Transform",
			sol::constructors<LuaTransformComponent(TransformComponent)>(),
			"position", sol::property(&LuaTransformComponent::GetPosition, &LuaTransformComponent::SetPosition),
			"dimension", sol::property(&LuaTransformComponent::GetDimension, &LuaTransformComponent::SetDimension)
			);

		lua.new_usertype<LuaMovementComponent>("Movement",
			sol::constructors<LuaMovementComponent(MovementComponent)>(),
			"speed", sol::property(&LuaMovementComponent::GetSpeed, &LuaMovementComponent::SetSpeed),
			"jump", sol::property(&LuaMovementComponent::GetJump, &LuaMovementComponent::SetJump)
			);

		lua.new_usertype<LuaPhysicsComponent>("Physics",
			sol::constructors<LuaPhysicsComponent(PhysicsComponent)>(),
			"velocity", sol::property(&LuaPhysicsComponent::GetVelocity, &LuaPhysicsComponent::SetVelocity),
			"Drop", &LuaPhysicsComponent::Drop,
			"CollidesBottom", &LuaPhysicsComponent::CollidesBottom
			);

		lua.new_usertype<LuaCameraComponent>("Camera",
			sol::constructors<LuaCameraComponent(CameraComponent)>(),
			"SetView", &LuaCameraComponent::SetView
			);

		lua.script_file(path);

		script = lua[funtionName];
	}
};