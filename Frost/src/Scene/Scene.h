#pragma once

#include "Tile/Tilemap.h"

#include "Event/Event.h"
#include "Script/Lua/LuaBinding.h"

#include "Entity/EntityMananger.h"

#include "Utility/Average.h"

class SceneManager;

class Scene
{
protected:
	std::string m_name;

	sol::state m_lua;
	unique_ptr<TileMap> m_map;
	std::map<std::string, unique_ptr<Entity>> m_entities;

	// stores the time the simulation takes for debug purposes
	Average<float> m_simTime;

	Scene(const std::string& name, TileMap* map);
public:
	~Scene();

	void SetName(const std::string& name);

	void AddEntity(const std::string& name, Entity* entity);
	Entity* GetEntity(const std::string& name) const;

	void OnUpdate();
	void OnRender();
	void OnRenderDebug();
	void OnImGui();

	TileMap* GetMap() const;
	Rect GetConstraint() const;

	sol::state& GetLua();

	std::string GetName() const;

	friend SceneManager;
};