#include "ComponentTable.h"

void ComponentTableInit(ComponentTable* table, size_t initial_size)
{
	for (size_t i = 0; i < NUM_COMPONENT_TYPES; ++i)
	{
		clib_dict_alloc(&table->components[i], initial_size);
	}
}

void ComponentTableFree(ComponentTable* table)
{
	ComponentTableClear(table);
	for (size_t i = 0; i < NUM_COMPONENT_TYPES; ++i)
	{
		clib_dict_free(&table->components[i]);
	}
}

void ComponentTableClear(ComponentTable* table)
{
	/* animataion */
	CLIB_DICT_ITERATE_FOR(&table->components[COMPONENT_ANIMATION], iter)
	{
		AnimatorDestroy((Animator*)clib_dict_iter_get_value(iter));
	}

	for (size_t i = 0; i < NUM_COMPONENT_TYPES; ++i)
	{
		CLIB_DICT_ITERATE_FOR(&table->components[i], iter)
			free(clib_dict_iter_get_value(iter));
		clib_dict_clear(&table->components[i]);
	}
}

size_t GetComponentSize(ComponentType type)
{
	switch (type)
	{
	case COMPONENT_TRANSFORM:	return sizeof(Transform);
	case COMPONENT_RIGID_BODY:	return sizeof(RigidBody);
	case COMPONENT_MOVEMENT:	return sizeof(Movement);
	case COMPONENT_SPRITE:		return sizeof(Sprite);
	case COMPONENT_ANIMATION:	return sizeof(Animator);
	case COMPONENT_CAMERA:		return sizeof(CameraController);
	case COMPONENT_INTERACTOR:	return sizeof(Interactor);
	case COMPONENT_INTERACTION:	return sizeof(Interaction);
	default: return 0;
	}
}

void* ComponentTableAddComponent(ComponentTable* table, const char* entity, ComponentType type, void* component)
{
	size_t size = GetComponentSize(type);

	void* data = malloc(size);
	memcpy(data, component, size);

	return clib_dict_insert(&table->components[type], entity, data);
}

void* ComponentTableGetComponent(ComponentTable* table, const char* entity, ComponentType type)
{
	return clib_dict_find(&table->components[type], entity);
}
