#include "Scene.h"

#include "Ignis/Ignis.h"
#include "Obelisk/Obelisk.h"

using namespace ignis;

Scene::Scene(std::shared_ptr<Camera> camera)
{
	m_world = std::make_unique<World>("res/maps/map.json");
	m_camera = camera;

	m_smoothMovement = 0.5f;
}

Scene::~Scene()
{

}

void Scene::AddEntity(std::shared_ptr<Entity> entity)
{
	auto phys = entity->GetComponent<PhysicsComponent>();

	if (phys != nullptr)
	{
		m_world->AddBody(phys->GetBody());
	}

	m_entities.push_back(entity);
}

void Scene::AddEntity(std::shared_ptr<Entity> entity, const glm::vec2& position)
{
	entity->SetPosition(position);
	AddEntity(entity);
}

void Scene::RemoveEntity(const std::string& name)
{
	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [&](auto& e) 
	{ 
		return obelisk::StringCompare(e->GetName(), name); 
	}), m_entities.end());
}

void Scene::Clear()
{
	m_entities.clear();
}

void Scene::OnEvent(Event& e)
{
}

void Scene::OnUpdate(float deltaTime)
{
	m_world->Tick(deltaTime);

	for (auto& entity : m_entities)
		entity->OnUpdate(this, deltaTime);
}

void Scene::OnRender()
{
	m_world->Render(glm::vec3(), m_camera->GetViewProjection());

	Renderer2D::Start(m_camera->GetViewProjection());

	for (auto& entity : m_entities)
		entity->OnRender(this);

	Renderer2D::Flush();
}

void Scene::OnRenderDebug()
{
	Primitives2D::Start(m_camera->GetViewProjection());

	for (auto& entity : m_entities)
	{
		entity->OnRenderDebug();
		//Primitives2D::DrawCircle(entity->GetPosition(), 2.0f);
	}

	for (auto& body : m_world->GetBodies())
		Primitives2D::DrawRect(body->GetPosition() - body->GetHalfDimension(), body->GetDimension());

	//Primitives2D::DrawCircle(m_camera->GetPosition(), 2.0f);
	//
	//// smooth movement area
	//float smooth_w = (m_camera->GetSize().x / 2.0f) * m_smoothMovement;
	//float smooth_h = (m_camera->GetSize().y / 2.0f) * m_smoothMovement;
	//
	//Primitives2D::DrawRect(m_camera->GetPosition().x - smooth_w, m_camera->GetPosition().y - smooth_h, smooth_w * 2.0f, smooth_h * 2.0f);

	Primitives2D::Flush();
}

void Scene::SetCameraPosition(const glm::vec3& position)
{
	float smooth_w = (m_camera->GetSize().x / 2.0f) * m_smoothMovement;
	float smooth_h = (m_camera->GetSize().y / 2.0f) * m_smoothMovement;

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