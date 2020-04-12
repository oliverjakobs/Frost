#include "ShadowMapper.h"

CLIB_VECTOR_DEFINE_FUNCS(shadow, EcsEntity)

int ShadowMapperInit(ShadowMapper* mapper)
{
	clib_vector_init(&mapper->entites, 8 * sizeof(EcsEntity));

	mapper->edge_count = 0;

	return 1;
}

void ShadowMapperDestroy(ShadowMapper* mapper)
{
	clib_vector_free(&mapper->entites);
}

void ShadowMapperAddEntity(ShadowMapper* mapper, EcsEntity* entity)
{
	if (entity->shadow)
	{
		shadow_vector_push(&mapper->entites, entity);
		mapper->edge_count += entity->shadow->edge_count;
	}
}

line* ShadowMapperGetEdges(ShadowMapper* mapper, size_t* edge_count)
{
	line* edges = (line*)malloc(sizeof(line) * mapper->edge_count);
	size_t edge_index = 0;

	for (size_t i = 0; i < mapper->entites.size; ++i)
	{
		EcsEntity* entity = shadow_vector_get(&mapper->entites, i);
		vec2 pos = EcsEntityGetPosition(entity);

		for (size_t e = 0; e < entity->shadow->edge_count; ++e)
		{
			edges[edge_index].start = vec2_add(pos, entity->shadow->edges[e].start);
			edges[edge_index].end = vec2_add(pos, entity->shadow->edges[e].end);

			edge_index++;
		}
	}
	*edge_count = edge_index;

	return edges;
}
