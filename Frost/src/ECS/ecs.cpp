#include "ecs.h"

#include <algorithm>

void ECS::addComponent(EntityHandle handle, std::vector<std::pair<uint32, uint32>>& entity, uint32 componentID, BaseECSComponent* component)
{
	ECSComponentCreateFunction createFn = BaseECSComponent::GetTypeCreateFunction(componentID);
	std::pair<uint32, uint32> compPair;
	compPair.first = componentID;
	compPair.second = createFn(m_components[componentID], handle, component);
	entity.push_back(compPair);
}

bool ECS::removeComponent(EntityHandle handle, uint32 componentID)
{
	Entity& entity = handleToEntity(handle);

	for (uint32 i = 0; i < entity.size(); i++)
	{
		if (componentID == entity[i].first)
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

BaseECSComponent* ECS::getComponent(std::vector<std::pair<uint32, uint32>>& entity, std::vector<uint8>& components, uint32 componentID)
{
	for (auto& c : entity)
	{
		if (componentID == c.first)
			return (BaseECSComponent*)&components[c.second];
	}

	return nullptr;
}

uint32 ECS::findLeastCommonComponent(const std::vector<uint32>& compTypes, const std::vector<uint32>& compFlags)
{
	uint32 minSize = (uint32) - 1;
	uint32 minIndex = (uint32) - 1;

	for (uint32 i = 0; i < compTypes.size(); i++)
	{
		if ((compFlags[i] & BaseECSSystem::FLAG_OPTIONAL) != 0)
			continue;

		size_t typeSize = BaseECSComponent::GetTypeSize(compTypes[i]);
		uint32 size = m_components[compTypes[i]].size() / typeSize;

		if (size <= minSize)
		{
			minSize = size;
			minIndex = i;
		}
	}

	return minIndex;
}

void ECS::deleteComponent(uint32 componentID, uint32 index)
{
	ECSComponentFreeFunction freeFn = BaseECSComponent::GetTypeFreeFunction(componentID);
	size_t typeSize = BaseECSComponent::GetTypeSize(componentID);

	std::vector<uint8>& compArray = m_components[componentID];
	uint32 srcIndex = compArray.size() - typeSize;

	BaseECSComponent* destComponent = (BaseECSComponent*)&compArray[index];
	BaseECSComponent* srcComponent = (BaseECSComponent*)&compArray[srcIndex];
	freeFn(destComponent);

	if (index == srcIndex)
	{
		compArray.resize(srcIndex);
		return;
	}

	memcpy(destComponent, srcComponent, typeSize);

	Entity& entity = handleToEntity(srcComponent->entity);

	for (auto& c : entity)
	{
		if (componentID == c.first && srcIndex == c.second)
		{
			c.second = index;
			break;
		}
	}

	compArray.resize(srcIndex);
}

void ECS::updateSystemWithMultipleComponents(BaseECSSystem* system, float deltaTime, std::vector<std::vector<uint8>*>& components, const std::vector<uint32>& compTypes, std::vector<BaseECSComponent*>& compParam)
{
	const std::vector<uint32>& compFlags = system->getComponentFlags();

	compParam.resize(std::max(compParam.size(), compTypes.size()));
	components.resize(std::max(components.size(), compTypes.size()));

	for (uint32 i = 0; i < compTypes.size(); i++)
	{
		components[i] = &m_components[compTypes[i]];
	}

	uint32 minSizeIndex = findLeastCommonComponent(compTypes, compFlags);

	size_t typeSize = BaseECSComponent::GetTypeSize(compTypes[minSizeIndex]);
	std::vector<uint8>& compArray = *components[minSizeIndex];

	for (uint32 i = 0; i < compArray.size(); i += typeSize)
	{
		compParam[minSizeIndex] = (BaseECSComponent*)&compArray[i];
		std::vector<std::pair<uint32, uint32>>& entity = handleToEntity(compParam[minSizeIndex]->entity);

		bool isValid = true;
		for (uint32 j = 0; j < compTypes.size(); j++)
		{
			if (j == minSizeIndex)
				continue;

			compParam[j] = getComponent(entity, *components[j], compTypes[j]);

			if ((compParam[j] == nullptr) && ((compFlags[j] & BaseECSSystem::FLAG_OPTIONAL) == 0))
			{
				isValid = false;
				break;
			}
		}

		if (isValid)
			system->update(deltaTime, &compParam[0]);
	}
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

EntityHandle ECS::createEntity(BaseECSComponent** components, const uint32* componentIDs, size_t componentCount)
{
	std::pair<uint32, Entity>* entity = new std::pair<uint32, Entity>();

	EntityHandle handle = (EntityHandle)entity;

	for (uint32 i = 0; i < componentCount; i++)
	{
		if (!BaseECSComponent::IsTypeValid(componentIDs[i]))
		{
			delete entity;
			return NULL_ENTITY_HANDLE;
		}

		addComponent(handle, entity->second, componentIDs[i], components[i]);
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



void ECS::updateSystems(ECSSystemList& systems, float deltaTime)
{
	std::vector<BaseECSComponent*> componentParam;
	std::vector<std::vector<uint8>*> components;

	for (uint32 i = 0; i < systems.size(); i++)
	{
		const std::vector<uint32>& componentTypes = systems[i]->getComponentTypes();

		if (componentTypes.size() == 1)
		{
			size_t typeSize = BaseECSComponent::GetTypeSize(componentTypes[0]);
			std::vector<uint8>& compArray = m_components[componentTypes[0]];

			for (uint32 j = 0; j < compArray.size(); j += typeSize)
			{
				BaseECSComponent* component = (BaseECSComponent*)&compArray[j];
				systems[i]->update(deltaTime, &component);
			}
		}
		else
		{
			updateSystemWithMultipleComponents(systems[i], deltaTime, components, componentTypes, componentParam);
		}
	}
}
