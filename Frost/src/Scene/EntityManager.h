#pragma once

#include "Entity/Entity.h"
#include "Entity/PlayerComponent.h"
#include "Entity/ImageComponent.h"
#include "Entity/InteractionComponent.h"

class EntityManager : private Singleton<EntityManager>
{
private:
	std::map<std::string, unique_ptr<Entity>> m_entities;
public:
	static Entity* CreateEntity(const std::string& name, float x, float y, float w, float h);
	static void AddEntity(Entity* entity);

	static Entity* GetEntity(const std::string& name);
	static void RemoveEntity(const std::string& name);

	//static void AddComponent(const std::string& entity, Component* comp);
};