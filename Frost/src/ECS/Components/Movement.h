#ifndef MOVEMENT_H
#define MOVEMENT_H

typedef enum
{
	MOVEMENT_RIGHT = 0,
	MOVEMENT_DOWN,
	MOVEMENT_LEFT,
	MOVEMENT_UP
} MovementDirection;

typedef struct
{
	MovementDirection direction;
	float speed;
	float jump_power;
} Movement;

#endif /* !MOVEMENT_H */