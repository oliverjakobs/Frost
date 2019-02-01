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
