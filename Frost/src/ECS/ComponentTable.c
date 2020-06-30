#include "ComponentTable.h"

#include "clib/hash.h"

void ComponentFree(void* block)
{
	free(block);
}

int ComponentTableInit(ComponentTable* components, size_t component_count)
{
	components->table = malloc(component_count * sizeof(clib_hashset));

	if (!components->table) return 0;

	components->count = component_count;
	for (size_t i = 0; i < component_count; ++i)
	{
		if (clib_hashset_alloc(&components->table[i], clib_hash_uint32, 0) == CLIB_HASHMAP_OK)
		{
			clib_hashset_set_value_alloc_funcs(&components->table[i], NULL, ComponentFree);
		}
	}

	return 1;
}

void ComponentTableFree(ComponentTable* components)
{
	ComponentTableClear(components);
	for (size_t i = 0; i < components->count; ++i)
	{
		clib_hashset_free(&components->table[i]);
	}
	free(components->table);
	components->count = 0;
}

int ComponentTableSetFreeFunc(ComponentTable* components, uint32_t type, void(*free_func)(void*))
{
	if (type >= components->count) return 0;

	clib_hashset_set_value_alloc_funcs(&components->table[type], NULL, ComponentFree);

	return 1;
}

void ComponentTableClear(ComponentTable* components)
{
	for (size_t i = 0; i < components->count; ++i)
	{
		clib_hashset_clear(&components->table[i]);
	}
}

void* ComponentTableAddComponent(ComponentTable* components, EntityID entity, uint32_t type, void* component)
{
	size_t size = ComponentsGetTypeSize(type);

	void* data = malloc(size);
	memcpy(data, component, size);

	return clib_hashset_insert(&components->table[type], entity, data);
}

void* ComponentTableGetComponent(ComponentTable* components, EntityID entity, uint32_t type)
{
	if (type >= components->count) return NULL;

	return clib_hashset_find(&components->table[type], entity);
}

void ComponentTableRemoveComponent(ComponentTable* components, EntityID entity, uint32_t type)
{
	if (type >= components->count) return;

	clib_hashset_remove(&components->table[type], entity);
}

void ComponentTableRemoveEntity(ComponentTable* components, EntityID entity)
{
	for (uint32_t i = 0; i < components->count; ++i)
	{
		ComponentTableRemoveComponent(components, entity, i);
	}
}
