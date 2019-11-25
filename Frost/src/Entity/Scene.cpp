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
	m_world->Render(glm::vec3(), m_camera->GetViewProjection());

	Renderer2D::Start(m_camera->GetViewProjection());

	for (auto& entity : m_entities)
		entity->OnRender();

	Renderer2D::Flush();
}

void Scene::OnRenderDebug()
{
	Primitives2D::Start(m_camera->GetViewProjection());

	for (auto& entity : m_entities)
	{
		entity->OnRenderDebug();
		Primitives2D::DrawCircle(entity->GetPosition(), 2.0f);
	}

	for (auto& body : m_world->GetBodies())
		Primitives2D::DrawRect(body->GetPosition() - body->GetHalfDimension(), body->GetDimension());

	Primitives2D::DrawCircle(m_camera->GetPosition(), 2.0f);

	// smooth movement area
	float smooth_w = m_camera->GetSize().x / 4.0f;
	float smooth_h = m_camera->GetSize().y / 4.0f;

	Primitives2D::DrawRect(m_camera->GetPosition().x - smooth_w, m_camera->GetPosition().y - smooth_h, smooth_w * 2.0f, smooth_h * 2.0f);

	Primitives2D::Flush();
}

void Scene::SetCameraPosition(const glm::vec3& position)
{
	float smooth_w = m_camera->GetSize().x / 4.0f;
	float smooth_h = m_camera->GetSize().y / 4.0f;

	glm::vec3 center = m_camera->GetPosition();

	if (position.x > m_camera->GetPosition().x + smooth_w)
		center.x = position.x - smooth_w;
	if (position.x < m_camera->GetPosition().x - smooth_w)
		center.x = position.x + smooth_w;

	if (position.y > m_camera->GetPosition().y + smooth_h)
		center.y = position.y - smooth_h;
	if (position.y < m_camera->GetPosition().y - smooth_h)
		center.y = position.y + smooth_h;

	// constraint
	float constraint_x = m_camera->GetSize().x / 2.0f;
	float constraint_y = m_camera->GetSize().y / 2.0f;

	if (center.x < constraint_x)
		center.x = constraint_x;
	if (center.x > GetWidth() - constraint_x)
		center.x = GetWidth() - constraint_x;

	if (center.y < constraint_y)
		center.y = constraint_y;
	if (center.y > GetHeight() - constraint_y)
		center.y = GetHeight() - constraint_y;

	m_camera->SetPosition(center);
}

std::shared_ptr<Entity> Scene::GetEntity(const std::string& name) const
{
	auto it = std::find_if(m_entities.begin(), m_entities.end(), [&](auto& e) { return obelisk::StringCompare(e->GetName(), name); });

	if (it != m_entities.end())
		return *it;

	return nullptr;
}