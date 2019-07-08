#include "Scene.h"

#include "ImGui/ImGuiRenderer.h"

#include "EntityMananger.h"

#include "Utility/Utils.h"
#include "Debugger.h"

Scene::Scene(const std::string& name, TileMap* map)
	: m_name(name), m_map(map)
{
	m_lua.LoadState(m_registry);
}

Scene::~Scene()
{
	for (auto&[name, entity] : m_entities)
	{
		m_registry.destroy(entity);
	}

	m_entities.clear();
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

	ScriptSystem::Tick(m_registry);
	m_lua.GetState().collect_garbage();

	TilePhysicsSystem::Tick(m_registry);
	AnimationSystem::Tick(m_registry);

	OnUserUpdate();
}

void Scene::OnRender()
{
	m_map->OnRender();

	ImageRenderSystem::Tick(m_registry);

	OnUserRender();
}

void Scene::OnRenderDebug()
{
	m_map->OnRenderDebug();

	OnUserRenderDebug();
}

void Scene::OnImGui()
{
	ImGui::Begin("Scene");
	ImGui::Text("Name: %s", m_name.c_str());
	ImGui::Separator();
	ImGui::Text("Lua:");
	ImGui::Text("Memory: %d bytes", m_lua.GetState().memory_used());
	ImGui::Separator();
	ImGui::Text("Map:");
	ImGui::Text("Size: %d, %d", m_map->GetWidth(), m_map->GetHeight());
	ImGui::Text("TileSize: %4.2f", m_map->GetTileSize());
	ImGui::Text("Gravity: %4.2f, %4.2f", m_map->GetGravity().x, m_map->GetGravity().y);
	ImGui::Text("Simulation time: %2.4f", m_map->GetSimulationTime());
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

LuaBinding& Scene::GetLua()
{
	return m_lua;
}

std::string Scene::GetName() const
{
	return m_name;
}