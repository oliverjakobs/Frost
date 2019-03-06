#pragma once

#include "ecsTypedef.h"

#include <tuple>

struct BaseECSComponent;

typedef uint32	(*ECSComponentCreateFunction)(std::vector<uint8>& memory, EntityHandle entity, BaseECSComponent* comp);
typedef void	(*ECSComponentFreeFunction)(BaseECSComponent* comp);

typedef std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t> ComponentTypeDef;

struct BaseECSComponent
{
private:
	static std::vector<ComponentTypeDef> s_componentTypes;
public:
	EntityHandle entity = NULL_ENTITY_HANDLE;
	
	static uint32 RegisterComponentType(ECSComponentCreateFunction createFn, ECSComponentFreeFunction freeFn, size_t size);
	
	static ECSComponentCreateFunction GetTypeCreateFunction(uint32 id);
	static ECSComponentFreeFunction GetTypeFreeFunction(uint32 id);
	static size_t GetTypeSize(uint32 id);

	static bool IsTypeValid(uint32 id);
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
uint32 ECSComponentCreate(std::vector<uint8>& memory, EntityHandle entity, BaseECSComponent* comp)
{
	// get the index where the component will be created
	uint32 index = memory.size();
	// resize the memory so the new component will fit
	memory.resize(index + CompType::SIZE);
	// create the component inside the memory
	CompType* component = new(&memory[index])CompType(*(CompType*)comp);
	// set the handle to the entity to which the component belongs
	component->entity = entity;

	return index;
}

template<typename CompType>
void ECSComponentFree(BaseECSComponent* comp)
{
	// cast to the actual component type
	CompType* component = (CompType*)comp;
	// and call the destructor
	component->~CompType();
}

template<typename T>
const uint32 ECSComponent<T>::ID(BaseECSComponent::RegisterComponentType(ECSComponentCreate<T>, ECSComponentFree<T>, sizeof(T)));

template<typename T>
const size_t ECSComponent<T>::SIZE(sizeof(T));

template<typename T>
const ECSComponentCreateFunction ECSComponent<T>::CREATE_FUNCTION(ECSComponentCreate<T>);

template<typename T>
const ECSComponentFreeFunction ECSComponent<T>::FREE_FUNCTION(ECSComponentFree<T>);
