#include "Scene.hpp"

#include "Renderer/Primitives.hpp"
#include "Obelisk/Obelisk.hpp"

Scene::Scene(std::shared_ptr<Camera> camera, float w, float h)
	: m_camera(camera), m_width(w), m_height(h)
{	
	m_world = std::make_unique<World>(glm::vec2(0.0f, -980.0f));

	m_smoothMovement = 0.5f;
}

Scene::~Scene()
{

}

void Scene::AddEntity(std::shared_ptr<Entity> entity)
{
	if (entity == nullptr)
		return;

	auto phys = entity->GetComponent<PhysicsComponent>();

	if (phys != nullptr)
	{
		m_world->AddBody(phys->GetBody());
	}

	m_entities.push_back(entity);
}

void Scene::AddEntity(std::shared_ptr<Entity> entity, const glm::vec2& position)
{
	if (entity == nullptr)
		return;

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

void Scene::OnEntry()
{
}

void Scene::OnExtit()
{
}

void Scene::OnEvent(const Event& e)
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
		Primitives2D::DrawCircle(entity->GetPosition(), 2.0f, WHITE);
	}

	for (auto& body : m_world->GetBodies())
		Primitives2D::DrawRect(body->GetPosition() - body->GetHalfSize(), body->GetSize(), body->GetType() == BodyType::DYNAMIC ? GREEN : WHITE);

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

bool inside(const glm::vec2& min, const glm::vec2& max, const glm::vec2& p)
{
	if (p.x < min.x || p.y < min.y) return false;
	if (p.x > max.x || p.y > max.y) return false;

	return true;
}

std::shared_ptr<Entity> Scene::GetEntityAt(const glm::vec2& pos) const
{
	for (auto& entity : m_entities)
	{
		auto tex = entity->GetComponent<TextureComponent>();

		if (tex == nullptr)
			continue;

		glm::vec2 position = entity->GetPosition();

		glm::vec2 min = position - glm::vec2(tex->GetWidth() / 2.0f, 0.0f);
		glm::vec2 max = min + tex->GetDimension();

		if (inside(min, max, pos))
			return entity;
	}

	return nullptr;
}
