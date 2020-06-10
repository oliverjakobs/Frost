#include "Entity.h"

#include <stdlib.h>
#include <string.h>

void EcsEntityLoad(EcsEntity* entity, const char* name, const char* template)
{
	entity->name = (char*)malloc(strlen(name));
	strcpy(entity->name, name);

	entity->template = (char*)malloc(strlen(template));
	strcpy(entity->template, template);

	entity->z_index = 0;
}

void EcsEntityDestroy(EcsEntity* entity)
{
	free(entity->name);
	free(entity->template);

	entity->z_index = 0;
}