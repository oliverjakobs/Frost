#include "Entity.h"

static EcsEntityID _entity_counter = 0;

void EcsEntityResetIDCounter()
{
	_entity_counter = 0;
}

EcsEntityID EcsEntityGetNextID()
{
	return _entity_counter++;
}
