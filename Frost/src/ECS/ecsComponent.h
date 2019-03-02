#pragma once

#include "ecsTypedef.h"

#include <tuple>

struct BaseECSComponent;

typedef uint32	(*ECSComponentCreateFunction)(std::vector<uint8>& memory, EntityHandle entity, BaseECSComponent* comp);
typedef void	(*ECSComponentFreeFunction)(BaseECSComponent* comp);

struct BaseECSComponent
{
private:
	static std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>* s_componentTypes;
public:
	EntityHandle entity = NULL_ENTITY_HANDLE;
	
	static uint32 RegisterComponentType(ECSComponentCreateFunction createFn, ECSComponentFreeFunction freeFn, size_t size);
	
	inline static ECSComponentCreateFunction GetTypeCreateFunction(uint32 id)
	{
		return std::get<0>((*s_componentTypes)[id]);
	}

	inline static ECSComponentFreeFunction GetTypeFreeFunction(uint32 id)
	{
		return std::get<1>((*s_componentTypes)[id]);
	}

	inline static size_t GetTypeSize(uint32 id)
	{
		return std::get<2>((*s_componentTypes)[id]);
	}

	inline static bool IsTypeValid(uint32 id)
	{
		return id < s_componentTypes->size();
	}
};

template<typename T>
struct ECSComponent : public BaseECSComponent
{
	static const ECSComponentCreateFunction CREATE_FUNCTION;
	static const ECSComponentFreeFunction FREE_FUNCTION;

	static const uint32 ID;
	static const size_t SIZE;
};

template<typename CompType>
unsigned int ECSComponentCreate(std::vector<uint8>& memory, EntityHandle entity, BaseECSComponent* comp)
{
	uint32 index = memory.size();
	memory.resize(index + CompType::SIZE);
	CompType* component = new(&memory[index])CompType(*(CompType*)comp);
	component->entity = entity;

	return index;
}

template<typename CompType>
void ECSComponentFree(BaseECSComponent* comp)
{
	CompType* component = (CompType*)comp;
	component->~CompType();
}

template<typename T>
const unsigned int ECSComponent<T>::ID(BaseECSComponent::RegisterComponentType(ECSComponentCreate<T>, ECSComponentFree<T>, sizeof(T)));

template<typename T>
const size_t ECSComponent<T>::SIZE(sizeof(T));

template<typename T>
const ECSComponentCreateFunction ECSComponent<T>::CREATE_FUNCTION(ECSComponentCreate<T>);

template<typename T>
const ECSComponentFreeFunction ECSComponent<T>::FREE_FUNCTION(ECSComponentFree<T>);
