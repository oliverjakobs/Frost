#include "Scene.h"

#include "ImGui/ImGuiRenderer.h"

#include "Utility/Utils.h"
#include "Utility/Timer.h"
#include "Debugger.h"

Scene::Scene(const std::string& name, TileMap* map)
	: m_name(name), m_map(map)
{
	m_lua.LoadState();
}

Scene::~Scene()
{
	m_entities.clear();
}

void Scene::SetName(const std::string& name)
{
	m_name = name;
}

void Scene::AddEntity(const std::string& name, const std::string& path)
{
	Entity* entity = EntityManager::CreateEntity(this, path);
	entity->SetScene(this);

	m_entities.insert({ entity->GetName(), unique_ptr<Entity>(entity) });
}

void Scene::AddSpawnPosition(const std::string& entityName, const glm::vec2& position)
{
	m_spawnPosition.insert({ entityName, position });
}

Entity* Scene::GetEntity(const std::string& name) const
{
	if (m_entities.find(name) != m_entities.end())
		return m_entities.at(name).get();

	return nullptr;
}

void Scene::OnEntry()
{
	for (auto&[name, pos] : m_spawnPosition)
	{
		if (m_entities.find(name) != m_entities.end())
			m_entities.at(name)->SetPosition(pos);
	}
}

void Scene::OnExtit()
{
}

void Scene::OnEvent(Event& e)
{

}

void Scene::OnUpdate()
{
	// measure time for simulation
	float start = Timer::GetTimeMS();

	for (auto&[name, entity] : m_entities)
	{
		entity->OnUpdate();
	}

	m_lua.GetState().collect_garbage();

	float end = Timer::GetTimeMS();

	m_simTime = end - start;
}

void Scene::OnRender()
{
	m_map->OnRender();

	for (auto&[name, entity] : m_entities)
	{
		entity->OnRender();
	}
}

void Scene::OnRenderDebug()
{
	m_map->OnRenderDebug();

	for (auto&[name, entity] : m_entities)
	{
		entity->OnRenderDebug();
	}
}

void Scene::OnImGui()
{
	ImGui::Begin("Scene");
	ImGui::Text("Name: %s", m_name.c_str());
	ImGui::Text("Simulation time: %2.4f ms", m_simTime);
	ImGui::Separator();
	ImGui::Text("Lua:");
	ImGui::Text("Memory: %d bytes", m_lua.GetState().memory_used());
	ImGui::Separator();
	ImGui::Text("Map:");
	ImGui::Text("Size: %d, %d", m_map->GetWidth(), m_map->GetHeight());
	ImGui::Text("TileSize: %4.2f", m_map->GetTileSize());
	ImGui::Text("Gravity: %4.2f, %4.2f", m_map->GetGravity().x, m_map->GetGravity().y);
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

LuaBinding& Scene::GetLua()
{
	return m_lua;
}

std::string Scene::GetName() const
{
	return m_name;
}