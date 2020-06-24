#include "Components.h"

size_t ComponentsGetTypeSize(ComponentType type)
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
