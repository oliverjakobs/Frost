#pragma once

#include "EntityManager.h"
#include "TilePhysics/Tilemap.h"

class Scene
{
protected:
	std::vector<unique_ptr<Entity>> m_entities;

	unique_ptr<TileMap> m_map;
public:
	Scene(TileMap* map);
	~Scene();

	void addEntity(Entity* entity);
	void addEntity(Entity* entity, float x, float y);
	void addEntity(Entity* entity, const glm::vec2& pos);
	void deleteEntity(const std::string& name);

	virtual void onEntry() {};
	virtual void onExtit() {};

	void onInput();
	void onUpdate();
	void onRender() const;
	void onRenderDebug() const;

	virtual void onUserInput() {};
	virtual void onUserUpdate() {};
	virtual void onUserRender() const {};
	virtual void onUserRenderDebug() const {};

	TileMap* getMap() const;
	Rect getConstraint() const;

	std::vector<Entity*> getEntities(unsigned int flag) const;
	Entity* getEntity(unsigned int flag) const;
	Entity* getLastEntity(unsigned int flag) const;

	std::vector<Entity*> getEntities(const std::string& name = "") const;
	std::vector<Entity*> getOtherEntities(const std::string& name) const;
	Entity* getEntity(const std::string& name) const;
	Entity* getLastEntity(const std::string& name) const;
	Entity* getNearestEntity(Entity* e) const;
};