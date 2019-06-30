#include "Scene.h"

#include "ImGui/ImGuiRenderer.h"

#include "EntityMananger.h"

Scene::Scene(const std::string& name, TileMap* map)
	: m_name(name), m_map(map)
{
	m_scriptSystem = new ScriptSystem(m_registry);
	m_tilePhysicsSystem = new TilePhysicsSystem();
	m_animationSystem = new AnimationSystem();
	m_imageRenderSystem = new ImageRenderSystem();
}

Scene::~Scene()
{
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
	m_map->onUpdate();

	m_scriptSystem->Tick(m_registry);
	m_tilePhysicsSystem->Tick(m_registry);
	m_animationSystem->Tick(m_registry);

	OnUserUpdate();
}

void Scene::OnRender()
{
	m_map->onRender();

	m_imageRenderSystem->Tick(m_registry);

	OnUserRender();
}

void Scene::OnRenderDebug()
{
	m_map->onRenderDebug();

	OnUserRenderDebug();

	// ImGui
	ImGui::Begin("Scene");
	ImGui::Text("Name: %s", m_name.c_str());
	ImGui::Separator();
	ImGui::Text("Map:");
	ImGui::Text("Size: %d, %d", m_map->getWidth(), m_map->getHeight());
	ImGui::Text("TileSize: %f", m_map->getTileSize());
	ImGui::Text("Gravity: %f, %f", m_map->getGravity().x, m_map->getGravity().y);
	ImGui::Text("Simulation time: %f", m_map->getSimulationTime());
	ImGui::End();
}

TileMap* Scene::GetMap() const
{
	return m_map.get();
}

Rect Scene::GetConstraint() const
{
	return Rect(glm::vec2(), m_map->getDimension() * m_map->getTileSize());
}

entt::registry& Scene::GetRegistry()
{
	return m_registry;
}

std::string Scene::GetName() const
{
	return m_name;
}