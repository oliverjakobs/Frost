#include "Entity.h"

#include "Component.h"
#include "Scene/Scene.h"
#include "Scrapbook/Graphics.h"

Entity::Entity(const Entity& copy)
{
	m_name = copy.m_name;
	m_position = copy.m_position;
	m_dimension = copy.m_dimension;

	m_flags = copy.m_flags;
	m_delete = false;
	
	for (auto& c : copy.m_components)
	{
		addComponent(c->clone());
	}

	setScene(nullptr);
}

Entity::Entity(const std::string& name, float x, float y, float w, float h)
	: m_name(name), m_position(glm::vec2(x, y)), m_dimension(glm::vec2(w, h)), m_delete(false)
{

}

Entity::~Entity()
{
	for (auto& c : m_components)
	{
		SAFE_DELETE(c);
	}
	m_components.clear();

	m_scene = nullptr;

	DEBUG_MESSAGE("DELETE ENTITY: " << m_name);
}

void Entity::onInput()
{
	for (auto comp : m_components)
	{
		if (comp->isActive())
		{
			comp->onInput();
		}
	}
}

void Entity::onUpdate()
{
	for (auto comp : m_components)
	{
		if (comp->isActive())
		{
			comp->onUpdate();
		}
	}
}

void Entity::onRender() const
{
	for (auto comp : m_components)
	{
		if (comp->isActive())
		{
			comp->onRender();
		}
	}
}

void Entity::onRenderDebug() const
{
	sb::Renderer::DrawRect(m_position - glm::vec2(m_dimension.x / 2.0f, 0.0f), m_dimension, sb::CYAN);

	for (auto comp : m_components)
	{
		if (comp->isActive())
		{
			comp->onRenderDebug();
		}
	}
}

void Entity::setScene(Scene* scene)
{
	m_scene = scene;

	if (scene == nullptr)
		for (auto& comp : m_components) comp->unload();
	else
		for (auto& comp : m_components) comp->load();
}

Scene* Entity::getScene() const
{
	return m_scene;
}

void Entity::setFlag(unsigned int flag)
{
	m_flags.addFlag(flag);
}

bool Entity::hasFlag(unsigned int flag)
{
	return m_flags.hasFlag(flag);
}

void Entity::setPosition(glm::vec2 pos)
{
	m_position = pos;
}

glm::vec2 Entity::getPosition() const
{
	return m_position;
}

glm::vec2 Entity::getCenter() const
{
	return getPosition() + glm::vec2(0.0f, m_dimension.y / 2.0f);
}

float Entity::getWidth() const { return m_dimension.x; }

float Entity::getHeight() const { return m_dimension.y; }

glm::vec2 Entity::getSize() const
{
	return m_dimension;
}

std::string Entity::getName() const
{
	return m_name;
}



void Entity::kill()
{
	m_delete = true;
}

bool Entity::shouldDelete() const
{
	return m_delete;
}

bool Entity::overlap(const Entity* entity) const
{
	if (m_position.x >= (entity->m_position.x + entity->m_dimension.x) || entity->m_position.x >= (m_position.x + m_dimension.x) 
		|| m_position.y >= (entity->m_position.y + entity->m_dimension.y) || entity->m_position.y >= (m_position.y + m_dimension.y))
		return false;

	return true;
}

float Entity::getDistance(Entity* entity)
{
	return sb::distance(getPosition(), entity->getPosition());
}

Entity* Entity::getNearestEntity()
{
	return m_scene->getNearestEntity(this);
}

Entity* Entity::addComponent(Component* c)
{
	if (c->setEntity(this))
		m_components.push_back(c);

	return this;
}

std::vector<Component*> Entity::getComponents() const
{
	return m_components;
}