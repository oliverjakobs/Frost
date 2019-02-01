#pragma once

#include "ECS\Entity.h"
#include "Tilemap.h"

class Scene
{
private:
	std::vector<Entity*> m_entities;

	TileMap* m_map;
public:
	Scene(float w, float h, TileMap* map);
	~Scene();

	void addEntity(Entity* entity);

	void onInput();
	void onUpdate();
	void onRender();
	void onRenderDebug();

	TileMap* getMap() const;
};