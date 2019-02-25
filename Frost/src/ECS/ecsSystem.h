#pragma once

#include "ecsComponent.h"

class BaseECSSystem
{
private:
	std::vector<uint32> m_componentTypes;
	std::vector<uint32> m_componentFlags;
protected:
	void addComponentType(uint32 componentType, uint32 componentFlag = 0)
	{
		m_componentTypes.push_back(componentType);
		m_componentFlags.push_back(componentFlag);
	}
public:
	enum
	{
		FLAG_OPTIONAL = 1
	};
	
	virtual void update(float deltaTime, BaseECSComponent** componets) {};

	bool isValid()
	{
		for (auto f : m_componentFlags)
		{
			if ((f & FLAG_OPTIONAL) == 0)
				return true;
		}
		return false;
	}

	const std::vector<uint32>& getComponentTypes() { return m_componentTypes; }
	const std::vector<uint32>& getComponentFlags() { return m_componentFlags; }
};
