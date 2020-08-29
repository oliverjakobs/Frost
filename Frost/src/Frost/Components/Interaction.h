#ifndef INTERACTION_H
#define INTERACTION_H

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

#endif /* !INTERACTION_H */