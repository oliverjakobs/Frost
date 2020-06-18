#include "ComponentTable.h"

#include "Components.h"

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
	case COMPONENT_PHYSICS:		return sizeof(EcsPhysicsComponent);
	case COMPONENT_MOVEMENT:	return sizeof(EcsMovementComponent);
	case COMPONENT_TEXTURE:		return sizeof(EcsTextureComponent);
	case COMPONENT_ANIMATION:	return sizeof(Animator);
	case COMPONENT_CAMERA:		return sizeof(EcsCameraComponent);
	case COMPONENT_INTERACTOR:	return sizeof(EcsInteractorComponent);
	case COMPONENT_INTERACTION:	return sizeof(EcsInteractionComponent);
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

void ComponentTableSetEntityPosition(ComponentTable* table, const char* entity, vec2 pos)
{
	EcsPhysicsComponent* physics = ComponentTableGetComponent(table, entity, COMPONENT_PHYSICS);

	if (physics)
	{ 
		physics->body.position.x = pos.x + physics->body_x;
		physics->body.position.y = pos.y + physics->body_y;
		return;
	}

	Transform* transform = ComponentTableGetComponent(table, entity, COMPONENT_TRANSFORM);

	if (transform)
	{
		transform->x = pos.x;
		transform->y = pos.y;
	}
}

vec2 ComponentTableGetEntityPosition(ComponentTable* table, const char* entity)
{
	EcsPhysicsComponent* physics = ComponentTableGetComponent(table, entity, COMPONENT_PHYSICS);

	if (physics)
	{
		return vec2_sub(physics->body.position, (vec2) { physics->body_x, physics->body_y });
	}

	Transform* transform = ComponentTableGetComponent(table, entity, COMPONENT_TRANSFORM);
	if (transform)
	{
		return (vec2) { transform->x, transform->y };
	}

	return (vec2) { 0.0f, 0.0f };
}

vec2 ComponentTableGetEntityCenter(ComponentTable* table, const char* entity)
{
	EcsPhysicsComponent* physics = ComponentTableGetComponent(table, entity, COMPONENT_PHYSICS);
	if (physics)
	{
		return physics->body.position;
	}

	return ComponentTableGetEntityPosition(table, entity);
}

int ComponentTableGetZIndex(ComponentTable* table, const char* entity)
{
	Transform* transform = ComponentTableGetComponent(table, entity, COMPONENT_TRANSFORM);
	if (transform)
		return transform->z_index;

	return 0;
}
