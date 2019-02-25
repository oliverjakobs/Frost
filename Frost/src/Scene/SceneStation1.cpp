#include "SceneStation1.h"

#include "Entity/InteractionComponent.h"

SceneStation1::SceneStation1()
	: Scene(new TileMap(ResourceManager::GetImage("tileset"), "res/maps/station1.txt"))
{
}

void SceneStation1::onEntry()
{
	addEntity(EntityManager::GetEntity("wall"));
	addEntity(EntityManager::GetEntity("door")->addComponent(new InteractionComponent(0.0f, GLFW_KEY_W, []() 
	{
		SceneManager::ChangeScene("station2"); 
	})));
	addEntity(EntityManager::GetEntity("player"), 400, 300);
}

void SceneStation1::onExtit()
{
	m_entities.clear();
}