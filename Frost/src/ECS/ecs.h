#pragma once

#include "ecsSystemList.h"

#include <map>

typedef std::vector<std::pair<uint32, uint32>> Entity;

class ECS
{
private:
	// 
	std::map<uint32, std::vector<uint8>> m_components;
	std::vector<std::pair<uint32, Entity>*> m_entities;

	inline std::pair<uint32, Entity>* handleToRawType(EntityHandle handle)
	{
		return (std::pair<uint32, Entity>*)handle;
	}

	inline uint32 handleToEntityIndex(EntityHandle handle)
	{
		return handleToRawType(handle)->first;
	}

	inline Entity& handleToEntity(EntityHandle handle)
	{
		return handleToRawType(handle)->second;
	}

	void addComponent(EntityHandle handle, Entity& entity, uint32 componentID, BaseECSComponent* component);
	bool removeComponent(EntityHandle handle, uint32 componentID);

	BaseECSComponent* getComponent(Entity& entity, std::vector<uint8>& components, uint32 componentID);
	uint32 findLeastCommonComponent(const std::vector<uint32>& compTypes, const std::vector<uint32>& compFlags);

	void deleteComponent(uint32 componentID, uint32 index);

	void updateSystemWithMultipleComponents(BaseECSSystem* system, float deltaTime, std::vector<std::vector<uint8>*>& components, const std::vector<uint32>& compTypes, std::vector<BaseECSComponent*>& compParam);
public:
	ECS();
	~ECS();

	// ---------------| Entity |--------------------------------------
	EntityHandle createEntity(BaseECSComponent** components, const uint32* componentIDs, size_t componentCount);
	void removeEntity(EntityHandle handle);

	template<class A>
	EntityHandle createEntity(A& c1)
	{
		BaseECSComponent* components[] = { &c1 };
		uint32 componentIDs[] = { A::ID };
		return createEntity(components, componentIDs, 1);
	}

	template<class A, class B>
	EntityHandle createEntity(A& c1, B& c2)
	{
		BaseECSComponent* components[] = { &c1, &c2 };
		uint32 componentIDs[] = { A::ID, B::ID };
		return createEntity(components, componentIDs, 2);
	}

	template<class A, class B, class C>
	EntityHandle createEntity(A& c1, B& c2, C& c3)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3 };
		uint32 componentIDs[] = { A::ID, B::ID, C::ID };
		return createEntity(components, componentIDs, 3);
	}

	template<class A, class B, class C, class D>
	EntityHandle createEntity(A& c1, B& c2, C& c3, D& c4)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4 };
		uint32 componentIDs[] = { A::ID, B::ID, C::ID, D::ID };
		return createEntity(components, componentIDs, 4);
	}

	template<class A, class B, class C, class D, class E>
	EntityHandle createEntity(A& c1, B& c2, C& c3, D& c4, E& c5)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5 };
		uint32 componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID };
		return createEntity(components, componentIDs, 5);
	}

	template<class A, class B, class C, class D, class E, class F>
	EntityHandle createEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6 };
		uint32 componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID, F::ID };
		return createEntity(components, componentIDs, 6);
	}

	template<class A, class B, class C, class D, class E, class F, class G>
	EntityHandle createEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6, G& c7)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6, &c7 };
		uint32 componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID };
		return createEntity(components, componentIDs, 7);
	}

	template<class A, class B, class C, class D, class E, class F, class G, class H>
	EntityHandle createEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6, G& c7, H& c8)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8 };
		uint32 componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID, H::ID };
		return createEntity(components, componentIDs, 8);
	}

	template<class A, class B, class C, class D, class E, class F, class G, class H, class I>
	EntityHandle createEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6, G& c7, H& c8, I& c9)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8, &c9 };
		uint32 componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID, H::ID, I::ID };
		return createEntity(components, componentIDs, 9);
	}

	template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J>
	EntityHandle createEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6, G& c7, H& c8, I& c9, J& c10)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8, &c9, &c10 };
		uint32 componentIDs[] = { A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID, H::ID, I::ID, J::ID };
		return createEntity(components, componentIDs, 10);
	}

	// ---------------| Component |-----------------------------------
	template<class Component>
	inline void addComponent(EntityHandle handle, Component* component)
	{
		addComponent(handle, handleToEntity(handle), Component::ID, component);
	}

	template<class Component>
	inline bool removeComponent(EntityHandle handle)
	{
		return removeComponent(handle, Component::ID);
	}

	template<class Component>
	inline Component* getComponent(EntityHandle handle)
	{
		return (Component*)getComponent(handleToEntity(handle), m_components[Component::ID] Component::ID);
	}

	// ---------------| System |--------------------------------------
	void updateSystems(ECSSystemList& systems, float deltaTime);
};