#ifndef ECS_COMPONENT_TABLE_H
#define ECS_COMPONENT_TABLE_H

#include "clib/dictionary.h"

#include "Components/Animator.h"
#include "Components/Transform.h"
#include "Components/RigidBody.h"
#include "Components/Sprite.h"
#include "Components/Movement.h"
#include "Components/CameraController.h"
#include "Components/Interaction.h"

typedef enum
{
	COMPONENT_TRANSFORM = 0,
	COMPONENT_RIGID_BODY,
	COMPONENT_MOVEMENT,
	COMPONENT_SPRITE,
	COMPONENT_ANIMATION,
	COMPONENT_CAMERA,
	COMPONENT_INTERACTION,
	COMPONENT_INTERACTOR,
	NUM_COMPONENT_TYPES
} ComponentType;

typedef struct
{
	clib_hashmap components[NUM_COMPONENT_TYPES];
} ComponentTable;

void ComponentTableInit(ComponentTable* table, size_t initial_size);
void ComponentTableFree(ComponentTable* table);

void ComponentTableClear(ComponentTable* table);

void* ComponentTableAddComponent(ComponentTable* table, const char* entity, ComponentType type, void* component);
void* ComponentTableGetComponent(ComponentTable* table, const char* entity, ComponentType type);


#endif /* !ECS_COMPONENT_TABLE_H */