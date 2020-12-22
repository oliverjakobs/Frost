#ifndef INTERACTION_H
#define INTERACTION_H

#include "Scenes/Scene.h"

typedef enum
{
	INTERACTION_TYPE_TOGGLE,	/* toggleable, on/off */
	INTERACTION_TYPE_RANGE,		/* range based, 'undo' if out of range */
	INTERACTION_TYPE_TIMED		/* time based */
} InteractionType;

typedef enum
{
	INTERACTION_NONE,
	INTERACTION_TOGGLE_DOOR,
	INTERACTION_OPEN_INVENTORY,
} Interaction;

typedef struct
{
	InteractionType type;
	float min_radius;
	float max_radius;
	Interaction interaction;
	int key;
} Interactable;

void InteractableLoad(Scene* scene, EcsEntityID entity, vec2 pos, int z_index, char* json);

int DispatchInteraction(Ecs* ecs, EcsEntityID entity, Interaction interaction, int active);

#endif /* !INTERACTION_H */