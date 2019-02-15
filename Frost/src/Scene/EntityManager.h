#pragma once

#include "ECS/Entity.h"

class EntityManager : private Singleton<EntityManager>
{
private:
	std::map<std::string, Entity*> m_entities;
public:
	static void Free();

	static Entity* CreateEntity(const std::string& name, float x, float y, float w, float h);
	static void AddEntity(Entity* entity);

	static Entity* GetEntity(const std::string& name);
	static void RemoveEntity(const std::string& name);

	//static void AddComponent(const std::string& entity, Component* comp);
};