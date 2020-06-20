#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "Systems/PhysicsSystem.h"
#include "Systems/PlayerSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/InteractionSystem.h"

#include "Systems/RenderSystem.h"
#include "Systems/DebugRenderSystem.h"

typedef struct
{
	void (*update)(ComponentTable*, float);
} EcsUpdateSystem;

typedef struct
{
	void (*render)(ComponentTable*, const float*);
} EcsRenderSystem;

#endif /* !SYSTEMS_H */
