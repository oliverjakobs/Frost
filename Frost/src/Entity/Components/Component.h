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
	Component(Entity* entity) : m_entity(entity), m_active(false) {}
	virtual ~Component() {}

	Entity* GetEntity() const { return m_entity; }

	void SetActive(bool b) { m_active = b; }
	bool IsActive() const { return m_active; }

	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnRender() = 0;
	virtual void OnRenderDebug() {};
};