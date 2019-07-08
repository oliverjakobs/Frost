#include "Scene.h"

#include "ImGui/ImGuiRenderer.h"

#include "EntityMananger.h"

#include "Utility/Utils.h"
#include "Debugger.h"

void Scene::LoadLuaState()
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
		DEBUG_ASSERT(m_registry.has<PhysicsComponent>(entity), "[LUA] Entity has no PhysicsComponent");

		return m_registry.get<PhysicsComponent>(entity).body->GetVelocity();
	});
	m_lua.set_function("CollidesBottom", [&](unsigned int entity)
	{
		DEBUG_ASSERT(m_registry.has<PhysicsComponent>(entity), "[LUA] Entity has no PhysicsComponent");

		return m_registry.get<PhysicsComponent>(entity).body->CollidesBottom();
	});
	m_lua.set_function("Drop", [&](unsigned int entity)
	{
		DEBUG_ASSERT(m_registry.has<PhysicsComponent>(entity), "[LUA] Entity has no PhysicsComponent");

		m_registry.get<PhysicsComponent>(entity).body->Drop();
	});
	m_lua.set_function("SetVelocity", [&](unsigned int entity, const glm::vec2& vel)
	{
		DEBUG_ASSERT(m_registry.has<PhysicsComponent>(entity), "[LUA] Entity has no PhysicsComponent");

		m_registry.get<PhysicsComponent>(entity).body->SetVelocity(vel);
	});
	m_lua.set_function("PlayAnimation", [&](unsigned int entity, const std::string& animation, int flip)
	{
		DEBUG_ASSERT(m_registry.has<AnimationComponent>(entity), "[LUA] Entity has no AnimationComponent");
		DEBUG_ASSERT(m_registry.has<ImageComponent>(entity), "[LUA] Entity has no ImageComponent");

		AnimationSystem::PlayAnimation(animation, m_registry.get<AnimationComponent>(entity));

		if (flip > 0)
			m_registry.get<ImageComponent>(entity).image->setRenderFlip((RenderFlip)(flip - 1));
	});
	m_lua.set_function("SetView", [&](unsigned int entity)
	{
		DEBUG_ASSERT(m_registry.has<CameraComponent>(entity), "[LUA] Entity has no CameraComponent");
		DEBUG_ASSERT(m_registry.has<TransformComponent>(entity), "[LUA] Entity has no TransformComponent");

		auto camera = m_registry.get<CameraComponent>(entity);

		Renderer::SetViewCenter(m_registry.get<TransformComponent>(entity).position + camera.cameraOffset, camera.constraint);
	});
}

Scene::Scene(const std::string& name, TileMap* map)
	: m_name(name), m_map(map)
{
	m_scriptSystem = new ScriptSystem();
	m_tilePhysicsSystem = new TilePhysicsSystem();
	m_animationSystem = new AnimationSystem();
	m_imageRenderSystem = new ImageRenderSystem();

	LoadLuaState();
}

Scene::~Scene()
{
	for (auto&[name, entity] : m_entities)
	{
		m_registry.destroy(entity);
	}

	m_entities.clear();

	SAFE_DELETE(m_scriptSystem);
	SAFE_DELETE(m_tilePhysicsSystem);
	SAFE_DELETE(m_animationSystem);
	SAFE_DELETE(m_imageRenderSystem);
}

void Scene::SetName(const std::string& name)
{
	m_name = name;
}

void Scene::AddEntity(const std::string& name, const std::string& path)
{
	m_entities.insert({ name, EntityManager::CreateEntity(this, path) });
}

unsigned int Scene::GetEntity(const std::string& name) const
{
	if (m_entities.find(name) != m_entities.end())
		return m_entities.at(name);

	return 0;
}

void Scene::OnEvent(Event& e)
{
	OnUserEvent(e);
}

void Scene::OnUpdate()
{
	m_map->OnUpdate();

	m_scriptSystem->Tick(m_registry);
	m_lua.collect_garbage();

	m_tilePhysicsSystem->Tick(m_registry);
	m_animationSystem->Tick(m_registry);

	OnUserUpdate();
}

void Scene::OnRender()
{
	m_map->OnRender();

	m_imageRenderSystem->Tick(m_registry);

	OnUserRender();
}

void Scene::OnRenderDebug()
{
	m_map->OnRenderDebug();

	OnUserRenderDebug();

	// ImGui
	ImGui::Begin("Scene");
	ImGui::Text("Name: %s", m_name.c_str());
	ImGui::Separator();
	ImGui::Text("Lua:");
	ImGui::Text("Memory: %d bytes", m_lua.memory_used());
	ImGui::Separator();
	ImGui::Text("Map:");
	ImGui::Text("Size: %d, %d", m_map->GetWidth(), m_map->GetHeight());
	ImGui::Text("TileSize: %f", m_map->GetTileSize());
	ImGui::Text("Gravity: %f, %f", m_map->GetGravity().x, m_map->GetGravity().y);
	ImGui::Text("Simulation time: %f", m_map->GetSimulationTime());
	ImGui::End();
}

TileMap* Scene::GetMap() const
{
	return m_map.get();
}

Rect Scene::GetConstraint() const
{
	return Rect(glm::vec2(), m_map->GetDimension() * m_map->GetTileSize());
}

entt::registry& Scene::GetRegistry()
{
	return m_registry;
}

sol::state& Scene::GetLua()
{
	return m_lua;
}

std::string Scene::GetName() const
{
	return m_name;
}