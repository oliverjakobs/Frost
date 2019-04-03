#pragma once

#include "ecsTypes.h"

#include <unordered_map>

class ecsComponent;

class ecsEntity
{
private:
	ecsEntityID m_id;
	std::unordered_map<ecsComponentID, ecsComponent*> m_components;

public:
	explicit ecsEntity(ecsEntityID id) : m_id(id) {}

	ecsEntityID getID() const { return m_id; }
};