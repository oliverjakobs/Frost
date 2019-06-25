#pragma once

#include "TilePhysics/Tilemap.h"

#include "Event/Event.h"
#include "ECS/Systems.h"

class Scene
{
protected:
	std::string m_name;

	unique_ptr<TileMap> m_map;

	entt::registry m_registry;
public:
	Scene(const std::string& name, TileMap* map);
	~Scene();

	void SetName(const std::string& name);
	
	virtual void OnEntry() {};
	virtual void OnExtit() {};

	void OnEvent(Event& e);
	void OnUpdate();
	void OnRender();
	void OnRenderDebug();

	virtual void OnUserEvent(Event& e) {};
	virtual void OnUserUpdate() {};
	virtual void OnUserRender() const {};
	virtual void OnUserRenderDebug() const {};

	TileMap* GetMap() const;
	Rect GetConstraint() const;
	entt::registry& GetRegistry();
	std::string GetName() const;
};