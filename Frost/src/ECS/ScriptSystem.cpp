#include "ScriptSystem.h"

#include "Systems.h"

ScriptSystem::ScriptSystem(entt::registry& registry)
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

	// functions
	lua.set_function("GetVelocity", [&](unsigned int entity)
	{
		DEBUG_ASSERT(registry.has<PhysicsComponent>(entity), "[LUA] Entity has no PhysicsComponent");

		return registry.get<PhysicsComponent>(entity).body->getVelocity();
	});
	lua.set_function("CollidesBottom", [&](unsigned int entity)
	{
		DEBUG_ASSERT(registry.has<PhysicsComponent>(entity), "[LUA] Entity has no PhysicsComponent");

		return registry.get<PhysicsComponent>(entity).body->collidesBottom();
	});
	lua.set_function("Drop", [&](unsigned int entity)
	{
		DEBUG_ASSERT(registry.has<PhysicsComponent>(entity), "[LUA] Entity has no PhysicsComponent");

		registry.get<PhysicsComponent>(entity).body->drop();
	});
	lua.set_function("SetVelocity", [&](unsigned int entity, const glm::vec2& vel)
	{
		DEBUG_ASSERT(registry.has<PhysicsComponent>(entity), "[LUA] Entity has no PhysicsComponent");

		registry.get<PhysicsComponent>(entity).body->setVelocity(vel);
	});
	lua.set_function("PlayAnimation", [&](unsigned int entity, const std::string& animation, int flip)
	{
		DEBUG_ASSERT(registry.has<AnimationComponent>(entity), "[LUA] Entity has no AnimationComponent");
		DEBUG_ASSERT(registry.has<ImageComponent>(entity), "[LUA] Entity has no ImageComponent");

		AnimationSystem::PlayAnimation(animation, registry.get<AnimationComponent>(entity));

		if (flip > 0)
			registry.get<ImageComponent>(entity).image->setRenderFlip((RenderFlip)(flip - 1));
	});
	lua.set_function("SetView", [&](unsigned int entity)
	{
		DEBUG_ASSERT(registry.has<CameraComponent>(entity), "[LUA] Entity has no CameraComponent");
		DEBUG_ASSERT(registry.has<TransformComponent>(entity), "[LUA] Entity has no TransformComponent");

		auto camera = registry.get<CameraComponent>(entity);

		Renderer::SetViewCenter(registry.get<TransformComponent>(entity).position + camera.cameraOffset, camera.constraint);
	});
}

void ScriptSystem::Tick(entt::registry& registry)
{
	auto view = registry.view<ScriptComponent>();

	for (auto entity : view)
	{
		auto& script = view.get(entity);

		lua.script_file(script.script);

		sol::function func = lua["on_update"];

		func(entity);

		lua.collect_garbage();
	}
};