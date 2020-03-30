#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "Animation/Animator.hpp"
#include "Ignis/Ignis.h"
#include "Physics/World.hpp"

typedef struct
{
	float x;
	float y;
} ecs_PositionComponent;

typedef struct
{
	Body* body;
	float body_x;
	float body_y;
} ecs_PhysicsComponent;

typedef struct
{
	float movement_speed;
	float jump_power;
} ecs_MovemenetComponent;

typedef enum RenderFlip
{
	RENDER_FLIP_NONE = 0,
	RENDER_FLIP_HORIZONTAL,
	RENDER_FLIP_VERTICAL,
	RENDER_FLIP_BOTH
} render_flip;

typedef struct
{
	IgnisTexture* texture;
	size_t frame;

	float width;
	float height;

	render_flip render_flip;
} ecs_TextureComponent;

typedef struct
{
	Animator* animator;
} ecs_AnimationComponent;

#endif // !COMPONENTS_H
