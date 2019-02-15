#pragma once

#include <cstdlib>

#include "Entity.h"

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
