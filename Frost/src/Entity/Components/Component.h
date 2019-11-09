#pragma once

#include "Entity/Entity.h"

enum class ComponentType
{
	DATA,
	LOGIC,
	RENDER
};

class Component
{
protected:
	Entity* m_entity;
	bool m_active;

public:
	Component() : m_entity(nullptr), m_active(false) {}
	virtual ~Component() {}

	//virtual Component* clone() = 0;

	bool SetEntity(Entity* e) { m_entity = e; return m_entity != nullptr; }
	Entity* GetEntity() const { return m_entity; }

	void SetActive(bool b) { m_active = b; }
	bool IsActive() const { return m_active; }

	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnRender() = 0;
	virtual void OnRenderDebug() {};
};