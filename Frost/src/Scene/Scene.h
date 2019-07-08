#pragma once

#include "Tile/Tilemap.h"

#include "Event/Event.h"
#include "ECS/Systems.h"

class Scene
{
protected:
	std::string m_name;

	unique_ptr<TileMap> m_map;

	std::unordered_map<std::string, unsigned int> m_entities;

	entt::registry m_registry;
	LuaBinding m_lua;
public:
	Scene(const std::string& name, TileMap* map);
	~Scene();

	void SetName(const std::string& name);

	void AddEntity(const std::string& name, const std::string& path);
	unsigned int GetEntity(const std::string& name) const;
	
	void OnEntry();
	void OnExtit();

	void OnEvent(Event& e);
	void OnUpdate();
	void OnRender();
	void OnRenderDebug();
	void OnImGui();

	TileMap* GetMap() const;
	Rect GetConstraint() const;

	entt::registry& GetRegistry();
	LuaBinding& GetLua();

	std::string GetName() const;
};