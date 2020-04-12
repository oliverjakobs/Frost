#ifndef SHADOW_MAPPER_H
#define SHADOW_MAPPER_H

#include "ECS/Ecs.h"

#include "clib/vector.h"

typedef struct
{
	clib_vector entites; 
	size_t edge_count;
} ShadowMapper;

int ShadowMapperInit(ShadowMapper* mapper);
void ShadowMapperDestroy(ShadowMapper* mapper);

void ShadowMapperAddEntity(ShadowMapper* mapper, EcsEntity* entity);
line* ShadowMapperGetEdges(ShadowMapper* mapper, size_t* edge_count);


#endif /* !SHADOW_MAPPER_H */

