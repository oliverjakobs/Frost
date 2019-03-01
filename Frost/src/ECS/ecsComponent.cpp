#include "ecsComponent.h"

std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>* BaseECSComponent::s_componentTypes;

uint32 BaseECSComponent::RegisterComponentType(ECSComponentCreateFunction createFn, ECSComponentFreeFunction freeFn, size_t size)
{
	if (s_componentTypes == nullptr)
		s_componentTypes = new std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>();

	uint32 typeID = s_componentTypes->size();
	s_componentTypes->push_back(std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>(createFn, freeFn, size));

	return typeID;
}
