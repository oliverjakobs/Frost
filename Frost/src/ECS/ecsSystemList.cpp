#include "ecsSystemList.h"

bool ECSSystemList::addSystem(BaseECSSystem* system)
{
	if (!system->isValid())
		return false;

	m_systems.push_back(std::unique_ptr<BaseECSSystem>(system));
	return true;
}

bool ECSSystemList::removeSystem(uint32 index)
{
	if (index >= m_systems.size())
		return false;

	m_systems.erase(m_systems.begin() + index);
	return true;
}
