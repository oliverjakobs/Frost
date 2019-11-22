#pragma once

#include "Entity/Components.h"

class Scene
{
private:
	std::unique_ptr<World> m_world;

	std::vector<std::shared_ptr<Entity>> m_entities;

public:
	Scene();
	~Scene();

	void AddEntity(std::shared_ptr<Entity> entity);
	void RemoveEntity(const std::string& name);
	void Clear();

	void OnUpdate(float deltaTime);
	void OnRender();
	void OnRenderDebug();

	std::shared_ptr<Entity> GetEntity(const std::string& name) const;
	std::vector<std::shared_ptr<Entity>> GetEntities() const { return m_entities; };

	World* GetWorld() { return m_world.get(); }
};