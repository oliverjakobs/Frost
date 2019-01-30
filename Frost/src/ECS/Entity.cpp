#include "Entity.h"

#include "Component.h"

Entity::Entity(const std::string& name, float x, float y, float w, float h)
	: m_name(name), m_position(glm::vec2(x, y)), m_dimension(glm::vec2(w, h))
{

}

Entity::~Entity()
{

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
	for (auto comp : m_components)
	{
		if (comp->isActive())
		{
			comp->onRenderDebug();
		}
	}
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

void Entity::addComponent(Component* c)
{
	if (c->setEntity(this))
		m_components.push_back(c);
}

std::vector<Component*> Entity::getComponents() const
{
	return m_components;
}