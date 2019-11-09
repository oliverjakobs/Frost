#include "Entity.h"

#include "Scene/Scene.h"
#include "Components/Component.h"

Entity::Entity(const std::string& name, const glm::vec2& position, const glm::vec2& dimension)
	: m_name(name), m_position(position), m_dimension(dimension), m_direction(Direction::RIGHT)
{

}

Entity::~Entity()
{
}

void Entity::OnUpdate(float deltaTime)
{
	for (auto& c : m_components)
	{
		c->OnUpdate(deltaTime);
	}
}

void Entity::OnRender()
{
	for (auto& c : m_components)
	{
		c->OnRender();
	}
}

void Entity::OnRenderDebug()
{
	for (auto& c : m_components)
	{
		c->OnRenderDebug();
	}
}

void Entity::SetPosition(const glm::vec2& pos)
{
	m_position = pos;
}

void Entity::SetDimension(const glm::vec2& dim)
{
	m_dimension = dim;
}

void Entity::SetDirection(Direction dir)
{
	m_direction = dir;
}

std::string Entity::GetName() const
{
	return m_name;
}

glm::vec2 Entity::GetPosition() const
{
	return m_position;
}

glm::vec2 Entity::GetDimension() const
{
	return m_dimension;
}

Direction Entity::GetDirection() const
{
	return m_direction;
}

void Entity::SetScene(Scene* scene)
{
	m_scene = scene;
}

Scene* Entity::GetScene() const
{
	return m_scene;
}

void Entity::AddComponent(Component* component)
{
	if (component->SetEntity(this))
		m_components.push_back(unique_ptr<Component>(component));
}
