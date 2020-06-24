#include "ComponentTable.h"

#include "clib/hash.h"

static size_t _component_table_hash_key(const void* key)
{
	return clib_hash_string((const char*)key);
}

static int _component_table_cmp_key(const void* a, const void* b)
{
	return strcmp((const char*)a, (const char*)b);
}

static void* _component_table_key_alloc(const void* src)
{
	size_t size = strlen(src);
	char* val = (char*)malloc(size + 1);

	if (!val) return NULL;

	strcpy(val, src);
	val[size] = '\0'; /* make sure key is null-terminated */

	return (void*)val;
}

static void _component_table_key_free(void* key)
{
	free(key);
}

void ComponentTableInit(ComponentTable* components, size_t initial_size)
{
	for (size_t i = 0; i < NUM_COMPONENT_TYPES; ++i)
	{
		if (clib_hashmap_alloc(&components->table[i], _component_table_hash_key, _component_table_cmp_key, initial_size) == CLIB_HASHMAP_OK)
			clib_hashmap_set_key_alloc_funcs(&components->table[i], _component_table_key_alloc, _component_table_key_free);
	}
}

void ComponentTableFree(ComponentTable* table)
{
	ComponentTableClear(table);
	for (size_t i = 0; i < NUM_COMPONENT_TYPES; ++i)
	{
		clib_hashmap_free(&table->table[i]);
	}
}

void ComponentTableClear(ComponentTable* table)
{
	/* animataion */
	CLIB_DICT_ITERATE_FOR(&table->table[COMPONENT_ANIMATION], iter)
	{
		AnimatorDestroy((Animator*)clib_dict_iter_get_value(iter));
	}

	for (size_t i = 0; i < NUM_COMPONENT_TYPES; ++i)
	{
		CLIB_DICT_ITERATE_FOR(&table->table[i], iter)
			free(clib_dict_iter_get_value(iter));
		clib_hashmap_clear(&table->table[i]);
	}
}

void* ComponentTableAddComponent(ComponentTable* table, const char* entity, ComponentType type, void* component)
{
	size_t size = ComponentsGetTypeSize(type);

	void* data = malloc(size);
	memcpy(data, component, size);

	return clib_hashmap_insert(&table->table[type], entity, data);
}

void* ComponentTableGetComponent(ComponentTable* table, const char* entity, ComponentType type)
{
	return clib_hashmap_find(&table->table[type], entity);
}

void ComponentTableRemoveComponent(ComponentTable* table, const char* entity, ComponentType type)
{
	/* TODO */
}

void ComponentTableRemoveEntity(ComponentTable* table, const char* entity)
{
	/* TODO */
}
