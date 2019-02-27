#include "Scene.h"

Scene::Scene(TileMap* map)
	: m_map(map)
{
}

Scene::~Scene()
{

}

void Scene::onInput()
{
	
}

void Scene::onUpdate()
{
	m_map->onUpdate();

	
}

void Scene::onRender() const
{
	m_map->onRender();

	
}

void Scene::onRenderDebug() const
{
	m_map->onRenderDebug();

	
}

TileMap* Scene::getMap() const
{
	return m_map.get();
}

Rect Scene::getConstraint() const
{
	return Rect(glm::vec2(), m_map->getDimension() * m_map->getTileSize());
}