#include "ComponentTable.h"

#include "clib/hash.h"

static size_t _component_table_hash_key(int32_t key)
{
	return clib_hash_int32((uint32_t)key);
}

void ComponentTableInit(ComponentTable* components, size_t initial_size)
{
	for (size_t i = 0; i < NUM_COMPONENT_TYPES; ++i)
	{
		clib_hashset_alloc(&components->table[i], _component_table_hash_key, initial_size);
	}
}

void ComponentTableFree(ComponentTable* table)
{
	ComponentTableClear(table);
	for (size_t i = 0; i < NUM_COMPONENT_TYPES; ++i)
	{
		clib_hashset_free(&table->table[i]);
	}
}

void ComponentTableClear(ComponentTable* table)
{
	/* animataion */
	CLIB_HASHSET_ITERATE_FOR(&table->table[COMPONENT_ANIMATION], iter)
	{
		AnimatorDestroy((Animator*)clib_hashset_iter_get_value(iter));
	}

	for (size_t i = 0; i < NUM_COMPONENT_TYPES; ++i)
	{
		CLIB_HASHSET_ITERATE_FOR(&table->table[i], iter)
			free(clib_hashset_iter_get_value(iter));
		clib_hashset_clear(&table->table[i]);
	}
}

void* ComponentTableAddComponent(ComponentTable* table, EntityID entity, ComponentType type, void* component)
{
	size_t size = ComponentsGetTypeSize(type);

	void* data = malloc(size);
	memcpy(data, component, size);

	return clib_hashset_insert(&table->table[type], entity, data);
}

void* ComponentTableGetComponent(ComponentTable* table, EntityID entity, ComponentType type)
{
	return clib_hashset_find(&table->table[type], entity);
}

void ComponentTableRemoveComponent(ComponentTable* table, EntityID entity, ComponentType type)
{
	clib_hashset_remove(&table->table[type], entity);
}

void ComponentTableRemoveEntity(ComponentTable* table, EntityID entity)
{
	for (size_t i = 0; i < NUM_COMPONENT_TYPES; ++i)
	{
		ComponentTableRemoveComponent(table, entity, i);
	}
}
