#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "Systems/PlayerSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/InteractionSystem.h"

#include "Systems/RenderSystem.h"
#include "Systems/DebugRenderSystem.h"


typedef struct
{
	void (*update)(ComponentTable*, const char*, float);
} EcsUpdateSystem;

typedef struct
{
	void (*render)(ComponentTable*, const char*);
	void (*pre)(const float*);
	void (*post)();
} EcsRenderSystem;

#endif /* !SYSTEMS_H */
