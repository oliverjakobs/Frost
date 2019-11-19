#include "Scene.h"

#include "ImGuiBinding/ImGuiRenderer.h"

#include "Utility/Utils.h"

Scene::Scene(const std::string& name, TileMap* map)
	: m_name(name), m_map(map)
{
	LoadLuaState(m_lua);
}

Scene::~Scene()
{
	m_entities.clear();
}

void Scene::SetName(const std::string& name)
{
	m_name = name;
}

void Scene::AddEntity(const std::string& name, Entity* entity)
{
	if (entity != nullptr)
	{
		entity->SetScene(this);

		m_entities.insert({ entity->GetName(), unique_ptr<Entity>(entity) });
	}
}

Entity* Scene::GetEntity(const std::string& name) const
{
	if (m_entities.find(name) != m_entities.end())
		return m_entities.at(name).get();

	return nullptr;
}

void Scene::OnUpdate(float deltaTime)
{
	// measure time for simulation
	//float start = Timer::GetTimeMS();

	for (auto&[name, entity] : m_entities)
	{
		entity->OnUpdate(deltaTime);
	}

	m_lua.collect_garbage();

	//float end = Timer::GetTimeMS();

	//m_simTime.Set(end - start);
}

void Scene::OnRender()
{
	m_world->Render();

	for (auto&[name, entity] : m_entities)
	{
		entity->OnRender();
	}
}

void Scene::OnRenderDebug()
{
	//m_map->OnRenderDebug();

	for (auto&[name, entity] : m_entities)
	{
		entity->OnRenderDebug();
	}
}

void Scene::OnImGui()
{
	ImGui::Begin("Scene");
	ImGui::Text("Name: %s", m_name.c_str());
	ImGui::Text("Simulation avg: %2.4f ms", m_simTime.GetAverage());
	ImGui::Text("Simulation max: %2.4f ms", m_simTime.GetMax());
	ImGui::Text("Simulation min: %2.4f ms", m_simTime.GetMin());
	ImGui::Separator();

	ImGui::Text("Lua:");
	ImGui::Text("Memory: %d bytes", m_lua.memory_used());
	ImGui::Separator();

	//ImGui::Text("Map:");
	//ImGui::Text("Size: %d, %d", m_map->GetWidth(), m_map->GetHeight());
	//ImGui::Text("TileSize: %4.2f", m_map->GetTileSize());
	//ImGui::Text("Gravity: %4.2f, %4.2f", m_map->GetGravity().x, m_map->GetGravity().y);
	//ImGui::End();
}

std::shared_ptr<World> Scene::GetWorld() const
{
	return m_world;
}

Line Scene::GetConstraint() const
{
	return Line();
	//return Rect(glm::vec2(), m_map->GetDimension() * m_map->GetTileSize());
}

sol::state& Scene::GetLua()
{
	return m_lua;
}

std::string Scene::GetName() const
{
	return m_name;
}