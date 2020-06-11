#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

#include "ComponentTable.h"

#include "Components/AnimationComponent.h"
#include "Ignis/Ignis.h"
#include "Physics/World.h"
#include "Camera/Camera.h"
#include "math/math.h"

typedef struct
{
	float x;
	float y;
} EcsPositionComponent;

typedef struct
{
	Body body;
	float body_x;
	float body_y;
} EcsPhysicsComponent;

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

typedef enum
{
	RENDER_FLIP_NONE = 0,
	RENDER_FLIP_HORIZONTAL,
	RENDER_FLIP_VERTICAL,
	RENDER_FLIP_BOTH
} RenderFlip;

typedef struct
{
	IgnisTexture2D* texture;
	size_t frame;

	float width;
	float height;

	RenderFlip render_flip;
} EcsTextureComponent;

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
