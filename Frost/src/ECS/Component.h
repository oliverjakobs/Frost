#pragma once

#include <cstdlib>

#include "Entity.h"

// TODO: Rework ECS
class Component
{
protected:
	Entity* m_entity;

	bool m_active;
public:
	Component() : m_entity(nullptr), m_active(true) {}
	virtual ~Component() {}

	virtual bool setEntity(Entity* e) { m_entity = e; return m_entity != nullptr; }
	virtual Entity* getEntity() { return m_entity; }

	virtual void setActive(bool a) { m_active = a; }
	virtual bool isActive() { return m_active; }

	virtual Component* clone() = 0;

	virtual void load() {};
	virtual void unload() {};

	virtual void onInput() = 0;
	virtual void onUpdate() = 0;
	virtual void onRender() const = 0;
	virtual void onRenderDebug() const {};
};

class SharedComponent : public Component
{
private:
	std::vector<Entity*> m_entities;
public:
	virtual bool setEntity(Entity* e) override { m_entities.push_back(e); return e != nullptr; }
	virtual std::vector<Entity*> getEntities() { return m_entities; }

	// set the entity that is currently using the component
	virtual void setActiveEntity(Entity* entity)
	{
		if (std::find(m_entities.begin(), m_entities.end(), entity) != m_entities.end())
		{
			m_entity = entity;
		}
	}

};