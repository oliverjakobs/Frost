#pragma once

#include "ECS/Entity.h"
#include "TilePhysics/Tilemap.h"

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

	std::vector<Entity*> getEntities(unsigned int flag) const;
	Entity* getEntity(unsigned int flag) const;
	Entity* getLastEntity(unsigned int flag) const;

	std::vector<Entity*> getEntities(const std::string& name = "") const;
	std::vector<Entity*> getOtherEntities(const std::string& name) const;
	Entity* getEntity(const std::string& name) const;
	Entity* getLastEntity(const std::string& name) const;
	Entity* getNearestEntity(Entity* e) const;
};