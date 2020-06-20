#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

#include "ComponentTable.h"

#include "Components/Animator.h"
#include "Components/Transform.h"
#include "Components/RigidBody.h"
#include "Components/Sprite.h"

#include "Ignis/Ignis.h"
#include "Camera/Camera.h"
#include "math/math.h"

typedef enum
{
	DIRECTION_RIGHT = 0,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_UP
} Direction;

typedef struct
{
	Direction direction;
	float speed;
	float jump_power;
} EcsMovementComponent;

typedef struct
{
	Camera* camera;
	float smooth;
	float scene_w;
	float scene_h;
} EcsCameraComponent;

typedef enum
{
	INTERACTION_SIMPLE
} InteractionType;

typedef struct
{
	InteractionType type;
} EcsInteractorComponent;

typedef struct
{
	float radius;
	InteractionType type;
} EcsInteractionComponent;

#endif /* !ECS_COMPONENTS_H */
