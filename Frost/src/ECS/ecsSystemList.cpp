#include "ecsSystemList.h"

bool ECSSystemList::addSystem(BaseECSSystem& system)
{
	if (!system.isValid())
		return false;

	m_systems.push_back(&system);
	return true;
}

bool ECSSystemList::removeSystem(BaseECSSystem& system)
{
	for (uint32 i = 0; i < m_systems.size(); i++)
	{
		if (&system == m_systems[i])
		{
			m_systems.erase(m_systems.begin() + i);
			return true;
		}
	}

	return false;
}
