#ifndef COMPONENTS_H
#define COMPONENTS_H

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
	COMPONENT_INTERACTOR
} ComponentType;


#endif /* !COMPONENTS_H */
