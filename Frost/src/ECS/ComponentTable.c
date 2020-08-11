#include "ComponentTable.h"

#include "clib/hash.h"

void ComponentFree(void* block)
{
	free(block);
}

int ComponentTableInit(ComponentTable* components, size_t component_count)
{
	clib_array_alloc(&components->table, component_count, sizeof(ComponentMap));

	return 1;
}

void ComponentTableFree(ComponentTable* components)
{
	ComponentTableClear(components);
	for (size_t i = 0; i < components->table.used; ++i)
	{
		ComponentMapFree(clib_array_get(&components->table, i));
	}
	clib_array_free(&components->table);
}

void ComponentTableClear(ComponentTable* components)
{
	for (size_t i = 0; i < components->table.used; ++i)
	{
		ComponentMapClear(clib_array_get(&components->table, i));
	}
}

ComponentType ComponentTableRegisterDataComponent(ComponentTable* components, size_t element_size, size_t initial_count, void(*free_func)(void*))
{
	ComponentMap comp;
	if (ComponentMapAlloc(&comp, free_func ? free_func : ComponentFree) == 0)
	{
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

	return ComponentMapInsert(clib_array_get(&components->table, type), entity, data);
}

void* ComponentTableGetDataComponent(ComponentTable* components, EntityID entity, ComponentType type)
{
	if (type >= components->table.used) return NULL;

	return ComponentMapFind(clib_array_get(&components->table, type), entity);
}

void ComponentTableRemoveDataComponent(ComponentTable* components, EntityID entity, ComponentType type)
{
	if (type >= components->table.used) return;

	ComponentMapRemove(clib_array_get(&components->table, type), entity);
}

void ComponentTableRemoveEntity(ComponentTable* components, EntityID entity)
{
	for (uint32_t i = 0; i < components->table.used; ++i)
	{
		ComponentTableRemoveDataComponent(components, entity, i);
	}
}
