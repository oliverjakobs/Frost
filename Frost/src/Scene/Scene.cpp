#include "Scene.h"

Scene::Scene(float w, float h, TileMap* map)
	: m_map(map)
{
}

Scene::~Scene()
{
	for (auto& e : m_entities)
	{
		SAFE_DELETE(e);
	}
	m_entities.clear();

	SAFE_DELETE(m_map);
}

void Scene::addEntity(Entity* entity)
{
	entity->setScene(this);
	m_entities.push_back(entity);
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

void Scene::onRender()
{
	m_map->onRender();

	for (auto& e : m_entities)
	{
		e->onRender();
	}
}

void Scene::onRenderDebug()
{
	m_map->onRenderDebug();

	for (auto& e : m_entities)
	{
		e->onRenderDebug();
	}
}

TileMap* Scene::getMap() const
{
	return m_map;
}

std::vector<Entity*> Scene::getEntities(unsigned int flag) const
{
	std::vector<Entity*> entities;

	for (auto e : m_entities)
	{
		if (e->hasFlag(flag))
			entities.push_back(e);
	}

	return entities;
}

Entity* Scene::getEntity(unsigned int flag) const
{
	for (auto e : m_entities)
	{
		if (e->hasFlag(flag))
			return e;
	}

	return nullptr;
}

Entity* Scene::getLastEntity(unsigned int flag) const
{
	Entity* entity = nullptr;

	for (auto e : m_entities)
	{
		if (e->hasFlag(flag))
			entity = e;
	}

	return entity;
}

std::vector<Entity*> Scene::getEntities(const std::string& name) const
{
	if (name.empty())
		return m_entities;

	std::vector<Entity*> entities;

	for (auto e : m_entities)
	{
		if (name.compare(e->getName()) == 0)
			entities.push_back(e);
	}

	return entities;
}

std::vector<Entity*> Scene::getOtherEntities(const std::string& name) const
{
	std::vector<Entity*> entities;

	for (auto e : m_entities)
	{
		if (name.compare(e->getName()) != 0)
			entities.push_back(e);
	}

	return entities;
}

Entity* Scene::getEntity(const std::string& name) const
{
	for (auto e : m_entities)
	{
		if (name.compare(e->getName()) == 0)
			return e;
	}

	return nullptr;
}

Entity* Scene::getLastEntity(const std::string& name) const
{
	Entity* entity = nullptr;

	for (auto e : m_entities)
	{
		if (name.compare(e->getName()) == 0)
			entity = e;
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