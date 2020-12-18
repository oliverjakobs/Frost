#ifndef INTERACTION_H
#define INTERACTION_H

#include "Scenes/Scene.h"

typedef enum
{
	INTERACTION_SIMPLE
} InteractionType;

typedef struct
{
	InteractionType type;
} Interactor;

typedef struct
{
	float radius;
	InteractionType type;
} Interaction;

void InteractorLoad(Scene* scene, EcsEntityID entity, vec2 pos, int z_index, char* json);
void InteractionLoad(Scene* scene, EcsEntityID entity, vec2 pos, int z_index, char* json);

#endif /* !INTERACTION_H */