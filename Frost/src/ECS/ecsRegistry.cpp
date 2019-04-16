#include "ecsRegistry.h"

namespace ecs
{
	Registry::Registry()
	{
	}

	Registry::~Registry()
	{
	}

	bool Registry::init()
	{
		return false;
	}

	void Registry::update()
	{
	}

	EntityID Registry::createEntity()
	{
		return INVALID_ENTITY_ID;
	}

	void Registry::destroyEntity(EntityID id)
	{
	}
}