#pragma once

#include "Tile/Tilemap.h"

#include "Event/Event.h"
#include "Script/Lua/LuaBinding.h"

#include "Entity/EntityMananger.h"

class Scene
{
protected:
	std::string m_name;

	unique_ptr<TileMap> m_map;

	std::map<std::string, unique_ptr<Entity>> m_entities;
	std::map<std::string, glm::vec2> m_spawnPosition;

	// stores the time the simulation takes for debug purposes
	float m_simTime;

	LuaBinding m_lua;
public:
	Scene(const std::string& name, TileMap* map);
	~Scene();

	void SetName(const std::string& name);

	void AddEntity(const std::string& name, const std::string& path);
	void AddSpawnPosition(const std::string& entityName, const glm::vec2& position);
	Entity* GetEntity(const std::string& name) const;
	
	void OnEntry();
	void OnExtit();

	void OnEvent(Event& e);
	void OnUpdate();
	void OnRender();
	void OnRenderDebug();
	void OnImGui();

	TileMap* GetMap() const;
	Rect GetConstraint() const;

	LuaBinding& GetLua();

	std::string GetName() const;
};