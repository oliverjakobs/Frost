#include "Scene.h"

Scene::Scene(float w, float h, TileMap* map)
	: m_map(map)
{
}

Scene::~Scene()
{

}

void Scene::addEntity(Entity* entity)
{
	if (entity != nullptr)
	{
		entity->setScene(this);
		m_entities.push_back(unique_ptr<Entity>(entity));
	}
}

void Scene::addEntity(Entity* entity, float x, float y)
{
	addEntity(entity, glm::vec2(x, y));
}

void Scene::addEntity(Entity* entity, const glm::vec2 & pos)
{
	if (entity != nullptr)
	{
		entity->setPosition(pos);
		entity->setScene(this);
		m_entities.push_back(unique_ptr<Entity>(entity));
	}
}

void Scene::deleteEntity(const std::string& name)
{
	auto it = std::find_if(m_entities.begin(), m_entities.end(), [&](auto& e) { return e->getName() == name; });

	if (it != m_entities.end())
	{
		m_entities.erase(it);
	}
}

void Scene::onInput()
{
	for (auto& e : m_entities)
	{
		e->onInput();
	}
}

void Scene::onUpdate()
{
	m_map->onUpdate();

	for (auto& e : m_entities)
	{
		e->onUpdate();
	}
}

void Scene::onRender() const
{
	m_map->onRender();

	for (auto& e : m_entities)
	{
		e->onRender();
	}
}

void Scene::onRenderDebug() const
{
	m_map->onRenderDebug();

	for (auto& e : m_entities)
	{
		e->onRenderDebug();
	}
}

TileMap* Scene::getMap() const
{
	return m_map.get();
}

Rect Scene::getConstraint() const
{
	return Rect(glm::vec2(), m_map->getDimension() * m_map->getTileSize());
}

std::vector<Entity*> Scene::getEntities(unsigned int flag) const
{
	std::vector<Entity*> entities;

	for (auto& e : m_entities)
	{
		if (e->hasFlag(flag))
			entities.push_back(e.get());
	}

	return entities;
}

Entity* Scene::getEntity(unsigned int flag) const
{
	for (auto& e : m_entities)
	{
		if (e->hasFlag(flag))
			return e.get();
	}

	return nullptr;
}

Entity* Scene::getLastEntity(unsigned int flag) const
{
	Entity* entity = nullptr;

	for (auto& e : m_entities)
	{
		if (e->hasFlag(flag))
			entity = e.get();
	}

	return entity;
}

std::vector<Entity*> Scene::getEntities(const std::string& name) const
{
	/*if (name.empty())
		return m_entities;*/

	std::vector<Entity*> entities;

	for (auto& e : m_entities)
	{
		if (stringCompare(e->getName(), name))
			entities.push_back(e.get());
	}

	return entities;
}

std::vector<Entity*> Scene::getOtherEntities(const std::string& name) const
{
	std::vector<Entity*> entities;

	for (auto& e : m_entities)
	{
		if (!stringCompare(e->getName(), name))
			entities.push_back(e.get());
	}

	return entities;
}

Entity* Scene::getEntity(const std::string& name) const
{
	for (auto& e : m_entities)
	{
		if (name.compare(e->getName()) == 0)
			return e.get();
	}

	return nullptr;
}

Entity* Scene::getLastEntity(const std::string& name) const
{
	Entity* entity = nullptr;

	for (auto& e : m_entities)
	{
		if (name.compare(e->getName()) == 0)
			entity = e.get();
	}

	return entity;
}

Entity* Scene::getNearestEntity(Entity* entity) const
{
	std::vector<Entity*> entities = getOtherEntities(entity->getName());

	if (entities.empty())
		return nullptr;

	Entity* nearestEntity = entities.front();

	for (auto e : entities)
	{
		if (entity->getDistance(e) < entity->getDistance(nearestEntity))
			nearestEntity = e;
	}

	return nearestEntity;
}