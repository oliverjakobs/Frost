#pragma once

#include "ecsSystem.h"

class ECSSystemList
{
private:
	std::vector<BaseECSSystem*> m_systems;
public:
	bool addSystem(BaseECSSystem& system);
	bool removeSystem(uint32 index);

	inline size_t size()
	{
		return m_systems.size();
	}

	inline BaseECSSystem* operator[](uint32 index)
	{
		return m_systems[index];
	}
};

