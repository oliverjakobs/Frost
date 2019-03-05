#include "ecs.h"

#include <algorithm>

void ECS::addComponent(EntityHandle handle, Entity& entity, uint32 typeID, BaseECSComponent* component)
{
	ECSComponentCreateFunction createFn = BaseECSComponent::GetTypeCreateFunction(typeID);
	entity.push_back(std::pair<uint32, uint32>(typeID, createFn(m_components[typeID], handle, component)));
}

bool ECS::removeComponent(EntityHandle handle, uint32 typeID)
{
	Entity& entity = handleToEntity(handle);

	for (uint32 i = 0; i < entity.size(); i++)
	{
		if (typeID == entity[i].first)
		{
			deleteComponent(entity[i].first, entity[i].second);

			uint32 srcIndex = entity.size() - 1;
			uint32 destIndex = i;

			entity[destIndex] = entity[srcIndex];
			entity.pop_back();

			return true;
		}
	}

	return false;
}

BaseECSComponent* ECS::getComponent(Entity& entity, uint32 typeID)
{
	for (auto& c : entity)
	{
		if (typeID == c.first)
			return (BaseECSComponent*)&m_components[typeID][c.second];
	}

	return nullptr;
}

uint32 ECS::findLeastCommonComponent(const std::vector<uint32>& types, const std::vector<uint32>& flags)
{
	// initialize minCount and minIndex to the max value
	uint32 minCount = (uint32) - 1;
	uint32 minIndex = (uint32) - 1;

	for (uint32 i = 0; i < types.size(); i++)
	{
		// if the component is optional ignore it 
		if ((flags[i] & ECSFlag::ECS_FLAG_OPTIONAL) != 0)
			continue;

		// take component size into account
		size_t typeSize = BaseECSComponent::GetTypeSize(types[i]);
		uint32 count = m_components[types[i]].size() / typeSize;

		if (count <= minCount)
		{
			minCount = count;
			minIndex = i;
		}
	}

	return minIndex;
}

void ECS::deleteComponent(uint32 typeID, uint32 index)
{
	// get the free function and the type size for the specific component type
	ECSComponentFreeFunction freeFn = BaseECSComponent::GetTypeFreeFunction(typeID);
	size_t typeSize = BaseECSComponent::GetTypeSize(typeID);

	// get all components of the right type
	std::vector<uint8>& compArray = m_components[typeID];
	// prepare to replace the component being deleted with the last one
	uint32 srcIndex = compArray.size() - typeSize;
	BaseECSComponent* destComponent = (BaseECSComponent*)&compArray[index];
	BaseECSComponent* srcComponent = (BaseECSComponent*)&compArray[srcIndex];

	// free the component
	freeFn(destComponent);

	// if the last component is being deleted ignore the next few lines
	if (index != srcIndex)
	{
		// move the last component into the delted components memory
		memcpy(destComponent, srcComponent, typeSize);

		// update the moved component index in its entity
		for (auto& c : handleToEntity(srcComponent->entity))
		{
			if (typeID == c.first && srcIndex == c.second)
			{
				c.second = index;
				break;
			}
		}
	}

	compArray.resize(srcIndex);
}

ECS::ECS()
{
}

ECS::~ECS()
{
	for (auto& c : m_components)
	{
		size_t typeSize = BaseECSComponent::GetTypeSize(c.first);
		ECSComponentFreeFunction freeFn = BaseECSComponent::GetTypeFreeFunction(c.first);

		for (uint32 i = 0; i < c.second.size(); i += typeSize)
			freeFn((BaseECSComponent*)&c.second[i]);
	}

	for (auto& e : m_entities)
	{
		delete e;
	}
}

EntityHandle ECS::createEntity(BaseECSComponent** components, const uint32* typeIDs, size_t componentCount)
{
	IndexedEntity* entity = new IndexedEntity();

	EntityHandle handle = (EntityHandle)entity;

	for (uint32 i = 0; i < componentCount; i++)
	{
		if (!BaseECSComponent::IsTypeValid(typeIDs[i]))
		{
			delete entity;
			return NULL_ENTITY_HANDLE;
		}

		addComponent(handle, entity->second, typeIDs[i], components[i]);
	}

	entity->first = m_entities.size();
	m_entities.push_back(entity);

	return handle;
}

void ECS::removeEntity(EntityHandle handle)
{
	auto entity = handleToEntity(handle);

	for (auto& c : entity)
		deleteComponent(c.first, c.second);

	uint32 destIndex = handleToEntityIndex(handle);
	uint32 srcIndex = m_entities.size() - 1;
	delete m_entities[destIndex];
	m_entities[destIndex] = m_entities[srcIndex];
	m_entities[destIndex]->first = destIndex;
	m_entities.pop_back();
}

void ECS::tickSystem(BaseECSSystem* system, float deltaTime)
{
	// get all the flags of the systems component
	const std::vector<uint32>& flags = system->getComponentFlags();
	// get a list of all required types for the system
	const std::vector<uint32>& types = system->getComponentTypes();
	uint32 leastCommonIndex = findLeastCommonComponent(types, flags);

	// get all components and the size of the least common type 
	std::vector<uint8>& leastCommon = m_components[types[leastCommonIndex]];
	size_t typeSize = BaseECSComponent::GetTypeSize(types[leastCommonIndex]);

	// initialize the vector for the components to update the system with
	std::vector<BaseECSComponent*> components(types.size());

	// loop over these components
	for (uint32 i = 0; i < leastCommon.size(); i += typeSize)
	{
		// get the least common component and put it at the right position 
		components[leastCommonIndex] = (BaseECSComponent*)&leastCommon[i];

		// get a reference to the entity to get the components of the other types
		Entity& entity = handleToEntity(components[leastCommonIndex]->entity);

		bool isValid = true;
		for (uint32 typeIndex = 0; typeIndex < types.size(); typeIndex++)
		{
			// if the component is of the least common type ignore it 
			if (typeIndex == leastCommonIndex)
				continue;

			// get the component of the specific type
			components[typeIndex] = getComponent(entity, types[typeIndex]);

			// if the entity misses a non-optional component it is not valid
			if ((components[typeIndex] == nullptr) && ((flags[typeIndex] & ECSFlag::ECS_FLAG_OPTIONAL) == 0))
			{
				isValid = false;
				break;
			}
		}

		// if the entity is valid update it 
		if (isValid)
			system->tick(components.data(), deltaTime);
	}
}

void ECS::tickSystems(ECSSystemList& systems, float deltaTime)
{
	for (uint32 i = 0; i < systems.size(); i++)
	{
		tickSystem(systems[i], deltaTime);
	}
}
