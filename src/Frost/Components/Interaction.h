#ifndef INTERACTION_H
#define INTERACTION_H

#include "Scenes/Scene.h"

/*
REQUIRES:
	- transform

PARSE_RULES:

InteractionType:
	"TOGGLE"	-> INTERACTION_TYPE_TOGGLE
	"RANGED"	-> INTERACTION_TYPE_RANGED
	"TIMED"		-> INTERACTION_TYPE_TIMED
	default		-> INTERACTION_TYPE_TOGGLE

Interaction:
	"TOGGLE_DOOR"		-> INTERACTION_TOGGLE_DOOR
	"OPEN_INVENTORY"	-> INTERACTION_OPEN_INVENTORY
	default				-> INTERACTION_NONE

JSON - Template:

"interaction": {
	"type": <InteractionType>,
	"interaction": <Interaction>,
	"key": <KeyName>,
	<IF type = INTERACTION_TYPE_TOGGLE>
		"range": [ <MIN:float>, <MAX:float> ]
	<IF type = INTERACTION_TYPE_RANGE>
		"range": [ <ACTIVATION:float>, <LEAVE:float> ]
	<IF type = INTERACTION_TYPE_TIMED>
		"range": [ <MIN:float>, <MAX:float> ],
		"time": <float>
}
*/

typedef enum
{
	INTERACTION_TYPE_TOGGLE,	/* toggleable, on/off */
	INTERACTION_TYPE_RANGED,	/* range based, 'undo' if out of range */
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
	Interaction interaction;
	int key;

	float range_min;
	float range_max;
	float time;
} Interactable;

void InteractableLoad(char* json, Ecs* ecs, EcsEntityID entity);

int InteractionToggleDoor(Ecs* ecs, EcsEntityID entity, int i);
int InteractionOpenInventory(Ecs* ecs, EcsEntityID entity, int active);

void DispatchInteraction(Ecs* ecs, EcsEntityID entity, const Interactable* interactable, float distance);

#endif /* !INTERACTION_H */