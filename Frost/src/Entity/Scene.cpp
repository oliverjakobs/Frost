#include "Scene.h"

#include "Ignis/Ignis.h"
#include "Obelisk/Obelisk.h"

using namespace ignis;

Scene::Scene(std::shared_ptr<Camera> camera)
{
	m_world = std::make_unique<World>("");
	m_camera = camera;
}

Scene::~Scene()
{

}

void Scene::AddEntity(std::shared_ptr<Entity> entity)
{
	m_entities.push_back(entity);
}

void Scene::RemoveEntity(const std::string& name)
{
	auto it = std::find_if(m_entities.begin(), m_entities.end(), [&](auto& e) { return obelisk::StringCompare(e->GetName(), name); });

	if (it != m_entities.end())
		m_entities.erase(it);
}

void Scene::Clear()
{
	m_entities.clear();
}

void Scene::OnUpdate(float deltaTime)
{
	m_world->Tick(deltaTime);

	for (auto& entity : m_entities)
		entity->OnUpdate(deltaTime);
}

void Scene::OnRender()
{
	for (auto& entity : m_entities)
		entity->OnRender();
}

void Scene::OnRenderDebug()
{
	for (auto& entity : m_entities)
	{
		entity->OnRenderDebug();
		Primitives2D::DrawCircle(entity->GetPosition(), 2.0f);
	}

	for (auto& body : m_world->GetBodies())
		Primitives2D::DrawRect(body->GetPosition() - body->GetHalfDimension(), body->GetDimension());
}

std::shared_ptr<Entity> Scene::GetEntity(const std::string& name) const
{
	auto it = std::find_if(m_entities.begin(), m_entities.end(), [&](auto& e) { return obelisk::StringCompare(e->GetName(), name); });

	if (it != m_entities.end())
		return *it;

	return nullptr;
}