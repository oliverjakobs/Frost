#include "EntityManager.h"

void EntityManager::Free()
{
	for (auto& e : Get()->m_entities)
	{
		SAFE_DELETE(e.second);
	}

	Get()->m_entities.clear();
}

Entity* EntityManager::CreateEntity(const std::string& name, float x, float y, float w, float h)
{
	Entity* entity = new Entity(name, x, y, w, h);

	Get()->m_entities[name] = entity;

	return entity;
}

void EntityManager::AddEntity(Entity* entity)
{
	Get()->m_entities[entity->getName()] = entity;
}

Entity* EntityManager::GetEntity(const std::string& name)
{
	try
	{
		return new Entity(*Get()->m_entities.at(name));
	}
	catch (std::out_of_range)
	{
		DEBUG_MESSAGE("No such entity: " << name);
		return nullptr;
	}
}

void EntityManager::RemoveEntity(const std::string& name)
{
	std::map<std::string, Entity*>::iterator it = Get()->m_entities.find(name);
	if (it != Get()->m_entities.end())
	{
		SAFE_DELETE(it->second);
		Get()->m_entities.erase(it);
	}
}
