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

void InteractorLoad(Scene* scene, EntityID entity, char* json);
void InteractionLoad(Scene* scene, EntityID entity, char* json);

#endif /* !INTERACTION_H */