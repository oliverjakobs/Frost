#pragma once

#include "Tile/World.h"

#include "Event/Event.h"
#include "Script/Lua/LuaBinding.h"

#include "Entity/EntityMananger.h"

#include "Utility/Average.h"

using namespace tile;

class SceneManager;

class Scene
{
protected:
	std::string m_name;

	sol::state m_lua;
	std::shared_ptr<World> m_world;
	std::map<std::string, unique_ptr<Entity>> m_entities;

	// stores the time the simulation takes for debug purposes
	Average<float> m_simTime;

	Scene(const std::string& name, TileMap* map);
public:
	~Scene();

	void SetName(const std::string& name);

	void AddEntity(const std::string& name, Entity* entity);
	Entity* GetEntity(const std::string& name) const;

	void OnUpdate(float deltaTime);
	void OnRender();
	void OnRenderDebug();
	void OnImGui();

	std::shared_ptr<World> GetWorld() const;
	Line GetConstraint() const;

	sol::state& GetLua();

	std::string GetName() const;

	friend SceneManager;
};