#include "ecsComponent.h"

std::vector<ComponentTypeDef> BaseECSComponent::s_componentTypes = std::vector<ComponentTypeDef>();

uint32 BaseECSComponent::RegisterComponentType(ECSComponentCreateFunction createFn, ECSComponentFreeFunction freeFn, size_t size)
{
	// set the typeID to the size of component types
	uint32 typeID = s_componentTypes.size();
	// add the type to the vector and so increase the typeID for the next component
	s_componentTypes.push_back(ComponentTypeDef(createFn, freeFn, size));
	
	return typeID;
}

ECSComponentCreateFunction BaseECSComponent::GetTypeCreateFunction(uint32 id)
{
	return std::get<0>(s_componentTypes[id]);
}

ECSComponentFreeFunction BaseECSComponent::GetTypeFreeFunction(uint32 id)
{
	return std::get<1>(s_componentTypes[id]);
}

size_t BaseECSComponent::GetTypeSize(uint32 id)
{
	return std::get<2>(s_componentTypes[id]);
}

bool BaseECSComponent::IsTypeValid(uint32 id)
{
	return (id < s_componentTypes.size());
}
