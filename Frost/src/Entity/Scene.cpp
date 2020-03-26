#include "Scene.hpp"

#include "Obelisk/Obelisk.hpp"

Scene::Scene(Camera* camera, float w, float h)
	: m_camera(camera), m_width(w), m_height(h)
{	
	m_world = std::make_unique<World>(glm::vec2(0.0f, -980.0f));

	m_smoothMovement = 0.5f;
}

Scene::~Scene()
{

}

void Scene::AddEntity(std::shared_ptr<Entity> entity, size_t layer)
{
	if (entity == nullptr)
		return;

	auto phys = entity->GetComponent<PhysicsComponent>();

	if (phys != nullptr)
	{
		m_world->AddBody(phys->GetBody());
	}

	m_entities[layer].push_back(entity);
}

void Scene::AddEntity(std::shared_ptr<Entity> entity, size_t layer, const glm::vec2& position)
{
	if (entity == nullptr)
		return;

	entity->SetPosition(position);
	AddEntity(entity, layer);
}

void Scene::RemoveEntity(const std::string& name, size_t layer)
{
	auto it = m_entities.find(layer);
	if (it != m_entities.end())
	{
		it->second.erase(std::remove_if(it->second.begin(), it->second.end(), [&](auto& e)
			{
				return obelisk::StringCompare(e->GetName(), name);
			}), it->second.end());
	}
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

	for (auto& layer : m_entities)
		for (auto& entity : layer.second)
			entity->OnUpdate(this, deltaTime);
}

void Scene::OnRender()
{
	BatchRenderer2DStart(CameraGetViewProjectionPtr(m_camera));

	for (auto& layer : m_entities)
		for (auto& entity : layer.second)
			entity->OnRender(this);

	BatchRenderer2DFlush();
}

void Scene::OnRenderDebug()
{
	Primitives2DStart(CameraGetViewProjectionPtr(m_camera));

	for (auto& layer : m_entities)
		for (auto& entity : layer.second)
		{
			entity->OnRenderDebug();

			auto& pos = entity->GetPosition();
			Primitives2DRenderCircle(pos.x, pos.y, 2.0f, IGNIS_WHITE);
		}

	for (auto& body : m_world->GetBodies())
	{
		auto& pos = body->GetPosition() - body->GetHalfSize();
		auto& dim = body->GetSize();
		Primitives2DRenderRect(pos.x, pos.y, dim.x, dim.y, body->GetType() == BodyType::DYNAMIC ? IGNIS_GREEN : IGNIS_WHITE);
	}

	//Primitives2D::DrawCircle(m_camera->GetPosition(), 2.0f);
	//
	//// smooth movement area
	//float smooth_w = (m_camera->GetSize().x / 2.0f) * m_smoothMovement;
	//float smooth_h = (m_camera->GetSize().y / 2.0f) * m_smoothMovement;
	//
	//Primitives2D::DrawRect(m_camera->GetPosition().x - smooth_w, m_camera->GetPosition().y - smooth_h, smooth_w * 2.0f, smooth_h * 2.0f);

	Primitives2DFlush();
}

void Scene::SetCameraPosition(const glm::vec3& position)
{
	float smooth_w = (m_camera->size.x / 2.0f) * m_smoothMovement;
	float smooth_h = (m_camera->size.y / 2.0f) * m_smoothMovement;

	glm::vec3 center = m_camera->position;

	if (position.x > m_camera->position.x + smooth_w)
		center.x = position.x - smooth_w;
	if (position.x < m_camera->position.x - smooth_w)
		center.x = position.x + smooth_w;

	if (position.y > m_camera->position.y + smooth_h)
		center.y = position.y - smooth_h;
	if (position.y < m_camera->position.y - smooth_h)
		center.y = position.y + smooth_h;

	// constraint
	float constraint_x = m_camera->size.x / 2.0f;
	float constraint_y = m_camera->size.y / 2.0f;

	if (center.x < constraint_x)
		center.x = constraint_x;
	if (center.x > GetWidth() - constraint_x)
		center.x = GetWidth() - constraint_x;

	if (center.y < constraint_y)
		center.y = constraint_y;
	if (center.y > GetHeight() - constraint_y)
		center.y = GetHeight() - constraint_y;

	m_camera->position = center;
	CameraUpdateViewOrtho(m_camera);
}

std::shared_ptr<Entity> Scene::GetEntity(const std::string& name, size_t layer) const
{
	auto it = m_entities.find(layer);
	if (it != m_entities.end())
	{
		auto entity = std::find_if(it->second.begin(), it->second.end(), [&](auto& e) { return obelisk::StringCompare(e->GetName(), name); });

		if (entity != it->second.end())
			return *entity;
	}

	return nullptr;
}

bool inside(const glm::vec2& min, const glm::vec2& max, const glm::vec2& p)
{
	if (p.x < min.x || p.y < min.y) return false;
	if (p.x > max.x || p.y > max.y) return false;

	return true;
}

std::shared_ptr<Entity> Scene::GetEntityAt(const glm::vec2& pos, size_t layer) const
{
	auto it = m_entities.find(layer);
	if (it != m_entities.end())
	{
		for (auto& entity : it->second)
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
	}	

	return nullptr;
}

std::vector<std::shared_ptr<Entity>> Scene::GetEntities(size_t layer) const
{
	auto it = m_entities.find(layer);
	if (it != m_entities.end())
	{
		return it->second;
	}

	return std::vector<std::shared_ptr<Entity>>();
}

std::vector<size_t> Scene::GetLayers() const
{
	std::vector<size_t> layers;
	for (auto& [key, value] : m_entities)
		layers.push_back(key);

	return layers;
}
