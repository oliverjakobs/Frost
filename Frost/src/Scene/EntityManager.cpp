#include "EntityManager.h"

void EntityManager::CreateEntity(const std::string& name, float x, float y, float w, float h, std::vector<Component*> components)
{
	Entity entity = Entity(name, x, y, w, h);

	for (auto& c : components)
	{
		entity.addComponent(c);
	}

	Get().m_entities[name] = std::make_unique<Entity>(entity);
}

void EntityManager::AddEntity(Entity* entity)
{
	Get().m_entities[entity->getName()] = unique_ptr<Entity>(entity);
}

Entity* EntityManager::GetEntity(const std::string& name)
{
	try
	{
		return new Entity(*Get().m_entities.at(name));
	}
	catch (std::out_of_range)
	{
		DEBUG_MESSAGE("No such entity: " << name);
		return nullptr;
	}
}

void EntityManager::RemoveEntity(const std::string& name)
{
	std::map<std::string, unique_ptr<Entity>>::iterator it = Get().m_entities.find(name);
	if (it != Get().m_entities.end())
	{
		Get().m_entities.erase(it);
	}
}
