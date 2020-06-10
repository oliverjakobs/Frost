#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

typedef struct
{
	char* name;
	char* template;

	int z_index;
} EcsEntity;

void EcsEntityLoad(EcsEntity* entity, const char* name, const char* template);
void EcsEntityDestroy(EcsEntity* entity);

#endif /* !ECS_ENTITY_H */
