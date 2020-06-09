#include "ComponentTable.h"


void ComponentTableInit(ComponentTable* table, size_t initial_size)
{
	for (size_t i = 0; i < NUM_COMPONENT_TYPES; ++i)
	{
		clib_dict_init(&table->components[i], initial_size);
	}
}

void ComponentTableFree(ComponentTable* table)
{
	ComponentTableClear(table);
	for (size_t i = 0; i < NUM_COMPONENT_TYPES; ++i)
	{
		clib_dict_destroy(&table->components[i]);
	}
}

void ComponentTableClear(ComponentTable* table)
{
	/* physics */
	CLIB_DICT_ITERATE_FOR(&table->components[COMPONENT_PHYSICS], iter)
		free(((EcsPhysicsComponent*)clib_dict_iter_get_value(iter))->body);

	/* animataion */
	CLIB_DICT_ITERATE_FOR(&table->components[COMPONENT_ANIMATION], iter)
	{
		AnimatorDestroy(((EcsAnimationComponent*)clib_dict_iter_get_value(iter))->animator);
		free(((EcsAnimationComponent*)clib_dict_iter_get_value(iter))->animator);
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
	case COMPONENT_POSITION: return sizeof(EcsPositionComponent);
	case COMPONENT_PHYSICS: return sizeof(EcsPhysicsComponent);
	case COMPONENT_MOVEMENT: return sizeof(EcsMovementComponent);
	case COMPONENT_TEXTURE: return sizeof(EcsTextureComponent);
	case COMPONENT_ANIMATION: return sizeof(EcsAnimationComponent);
	case COMPONENT_CAMERA: return sizeof(EcsCameraComponent);
	case COMPONENT_INTERACTION: return sizeof(EcsInteractionComponent);
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
	return clib_dict_get(&table->components[type], entity);
}
