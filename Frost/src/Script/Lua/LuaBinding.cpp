#include "LuaBinding.h"

#include "String/StringUtils.h"
#include "Input/Input.h"

#include "Debugger.h"
#include "Scene/Scene.h"

unsigned int LuaInput::GetKeyCode(const std::string& key)
{
	if (stringCompare(key, "SPACE"))
		return KEY_SPACE;

	return key.front();
}

bool LuaInput::KeyPressed(const std::string& key)
{
	return Input::KeyPressed(GetKeyCode(key));
}

bool LuaInput::KeyReleased(const std::string & key)
{
	return Input::KeyReleased(GetKeyCode(key));
}

void LuaBinding::LoadState(entt::registry& registry)
{
	m_lua.open_libraries(sol::lib::base);

	m_lua.new_usertype<glm::vec2>("Vec2",
		sol::constructors<glm::vec2(), glm::vec2(float, float)>(),
		"x", &glm::vec2::x,
		"y", &glm::vec2::y
		);

	// Input
	m_lua.new_usertype<LuaInput>("Input",
		sol::constructors<LuaInput()>(),
		"KeyPressed", &LuaInput::KeyPressed,
		"KeyReleased", &LuaInput::KeyReleased
		);

	// functions
	m_lua.set_function("GetVelocity", [&](unsigned int entity)
	{
		DEBUG_ASSERT(registry.has<PhysicsComponent>(entity), "[LUA] Entity has no PhysicsComponent");

		return registry.get<PhysicsComponent>(entity).body->GetVelocity();
	});
	m_lua.set_function("CollidesBottom", [&](unsigned int entity)
	{
		DEBUG_ASSERT(registry.has<PhysicsComponent>(entity), "[LUA] Entity has no PhysicsComponent");

		return registry.get<PhysicsComponent>(entity).body->CollidesBottom();
	});
	m_lua.set_function("Drop", [&](unsigned int entity)
	{
		DEBUG_ASSERT(registry.has<PhysicsComponent>(entity), "[LUA] Entity has no PhysicsComponent");

		registry.get<PhysicsComponent>(entity).body->Drop();
	});
	m_lua.set_function("SetVelocity", [&](unsigned int entity, const glm::vec2& vel)
	{
		DEBUG_ASSERT(registry.has<PhysicsComponent>(entity), "[LUA] Entity has no PhysicsComponent");

		registry.get<PhysicsComponent>(entity).body->SetVelocity(vel);
	});
	m_lua.set_function("PlayAnimation", [&](unsigned int entity, const std::string& animation, int flip)
	{
		DEBUG_ASSERT(registry.has<AnimationComponent>(entity), "[LUA] Entity has no AnimationComponent");
		DEBUG_ASSERT(registry.has<ImageComponent>(entity), "[LUA] Entity has no ImageComponent");

		AnimationSystem::PlayAnimation(animation, registry.get<AnimationComponent>(entity));

		if (flip > 0)
			registry.get<ImageComponent>(entity).image->setRenderFlip((RenderFlip)(flip - 1));
	});
	m_lua.set_function("SetView", [&](unsigned int entity)
	{
		DEBUG_ASSERT(registry.has<CameraComponent>(entity), "[LUA] Entity has no CameraComponent");
		DEBUG_ASSERT(registry.has<TransformComponent>(entity), "[LUA] Entity has no TransformComponent");

		auto camera = registry.get<CameraComponent>(entity);

		Renderer::SetViewCenter(registry.get<TransformComponent>(entity).position + camera.cameraOffset, camera.constraint);
	});

	m_lua.set_function("SetDirection", [&](unsigned int entity, const std::string& direction)
	{
		DEBUG_ASSERT(registry.has<TransformComponent>(entity), "[LUA] Entity has no TransformComponent");

		registry.get<TransformComponent>(entity).direction = Direction::FromString(direction);
	});
}

sol::function LuaBinding::BindLuaFunction(const std::string& src, const std::string& func)
{
	m_lua.script_file(src);

	return m_lua[func];
}

sol::state& LuaBinding::GetState()
{
	return m_lua;
}
