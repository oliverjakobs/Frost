#include "Entity.h"

static EntityID _entity_counter = 0;

EntityID EntityGetNextID()
{
	return _entity_counter++;
}
