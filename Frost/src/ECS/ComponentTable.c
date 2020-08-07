#include "ComponentTable.h"

#include "clib/hash.h"

void ComponentFree(void* block)
{
	free(block);
}

int ComponentTableInit(ComponentTable* components, size_t component_count)
{
	clib_array_alloc(&components->table, component_count, sizeof(clib_hashset));

	return 1;
}

void ComponentTableFree(ComponentTable* components)
{
	ComponentTableClear(components);
	for (size_t i = 0; i < components->table.used; ++i)
	{
		clib_hashset_free(clib_array_get(&components->table, i));
	}
	clib_array_free(&components->table);
}

void ComponentTableClear(ComponentTable* components)
{
	for (size_t i = 0; i < components->table.used; ++i)
	{
		clib_hashset_clear(clib_array_get(&components->table, i));
	}
}

ComponentType ComponentTableRegisterDataComponent(ComponentTable* components, size_t element_size, size_t initial_count, void(*free_func)(void*))
{
	clib_hashset comp;
	if (clib_hashset_alloc(&comp, clib_hash_uint32, 0) == CLIB_HASHSET_OK)
	{
		clib_hashset_set_value_alloc_funcs(&comp, NULL, free_func ? free_func : ComponentFree);

		if (clib_array_push(&components->table, &comp))
			return components->table.used - 1;
	}
	return 0;
}

void* ComponentTableAddDataComponent(ComponentTable* components, EntityID entity, ComponentType type, void* component)
{
	size_t size = ComponentsGetTypeSize(type);

	void* data = malloc(size);
	memcpy(data, component, size);

	return clib_hashset_insert(clib_array_get(&components->table, type), entity, data);
}

void* ComponentTableGetDataComponent(ComponentTable* components, EntityID entity, ComponentType type)
{
	if (type >= components->table.used) return NULL;

	return clib_hashset_find(clib_array_get(&components->table, type), entity);
}

void ComponentTableRemoveDataComponent(ComponentTable* components, EntityID entity, ComponentType type)
{
	if (type >= components->table.used) return;

	clib_hashset_remove(clib_array_get(&components->table, type), entity);
}

void ComponentTableRemoveEntity(ComponentTable* components, EntityID entity)
{
	for (uint32_t i = 0; i < components->table.used; ++i)
	{
		ComponentTableRemoveDataComponent(components, entity, i);
	}
}
