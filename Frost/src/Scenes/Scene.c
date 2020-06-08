#include "Scene.h"

#include "ECS/Systems.h"

int SceneLoad(Scene* scene, Camera* camera, float w, float h)
{
	scene->camera = camera;
	scene->width = w;
	scene->height = h;

	scene->world = (World*)malloc(sizeof(World));
	WorldLoad(scene->world, (vec2){ 0.0f, -980.0f });

	scene->smooth_movement = 0.5f;

	EcsInit(&scene->ecs, 2, 2);
	EcsAddUpdateSystem(&scene->ecs, EcsSystemPlayer);
	EcsAddUpdateSystem(&scene->ecs, EcsSystemAnimation);
	EcsAddRenderSystem(&scene->ecs, EcsSystemRender, EcsSystemRenderPre, EcsSystemRenderPost);
	EcsAddRenderSystem(&scene->ecs, EcsSystemDebugRender, EcsSystemDebugRenderPre, EcsSystemDebugRenderPost);
	
	clib_array_alloc(&scene->entities, 16, sizeof(EcsEntity));
	EcsComponentListInit(&scene->components, 16);

	return 1;
}

void SceneQuit(Scene* scene)
{
	BackgroundClear(&scene->background);

	SceneClearEntities(scene);
	clib_array_free(&scene->entities);

	WorldDestroy(scene->world);
	free(scene->world);

	EcsDestroy(&scene->ecs);
	EcsComponentListDelete(&scene->components);
}

int _SceneEntityCmp(const EcsEntity* a, const EcsEntity* b)
{
	return a->z_index - b->z_index;
}

void SceneAddEntity(Scene* scene, EcsEntity* entity, int z_index)
{
	if (!entity)
		return;

	EcsPhysicsComponent* phys = entity->physics;
	if (phys)
	{
		WorldAddBody(scene->world, phys->body);
	}

	EcsCameraComponent* cam = entity->camera;
	if (cam)
	{
		entity->camera->camera = scene->camera;
		entity->camera->scene_w = scene->width;
		entity->camera->scene_h = scene->height;
	}

	entity->z_index = z_index;
	clib_array_insert(&scene->entities, entity);

	/* sort by z_index */
	qsort((EcsEntity*)scene->entities.data, scene->entities.used, sizeof(EcsEntity), _SceneEntityCmp);
}

void SceneAddEntityPos(Scene* scene, EcsEntity* entity, int z_index, vec2 position)
{
	if (!entity) return;

	EcsEntitySetPosition(entity, position);
	SceneAddEntity(scene, entity, z_index);
}

void SceneRemoveEntity(Scene* scene, const char* name)
{
	for (size_t i = 0; i < scene->entities.used; i++)
	{
		EcsEntity* e = (EcsEntity*)clib_array_get(&scene->entities, i);
		if (strcmp(e->name, name) == 0)
		{
			/* todo: free components */
			clib_array_remove(&scene->entities, i);
			return;
		}
	}
}

void SceneClearEntities(Scene* scene)
{
	EcsComponentListClear(&scene->components);
	clib_array_clear(&scene->entities);
}

void SceneOnEvent(Scene* scene, Event e)
{
}

void SceneOnUpdate(Scene* scene, float deltaTime)
{
	WorldTick(scene->world, deltaTime);

	BackgroundUpdate(&scene->background, scene->camera->position.x - scene->camera->size.x / 2.0f, deltaTime);

	EcsUpdate(&scene->ecs, (EcsEntity*)scene->entities.data, scene->entities.used, deltaTime);
}

void SceneOnRender(Scene* scene)
{
	BackgroundRender(&scene->background, CameraGetViewProjectionPtr(scene->camera));

	EcsRender(&scene->ecs, (EcsEntity*)scene->entities.data, scene->entities.used, CameraGetViewProjectionPtr(scene->camera));
}

void SceneOnRenderDebug(Scene* scene)
{

}

EcsEntity* SceneGetEntity(Scene* scene, const char* name)
{
	for (size_t i = 0; i < scene->entities.used; ++i)
	{
		EcsEntity* entity = (EcsEntity*)clib_array_get(&scene->entities, i);
		if (strcmp(entity->name, name) == 0)
			return entity;
	}

	return NULL;
}

EcsEntity* SceneGetEntityAt(Scene* scene, vec2 pos)
{
	for (size_t i = 0; i < scene->entities.used; ++i)
	{
		EcsEntity* entity = (EcsEntity*)clib_array_get(&scene->entities, i);

		if (!entity->texture) continue;

		vec2 position = EcsEntityGetPosition(entity);

		vec2 min = vec2_sub(position, (vec2){ entity->texture->width / 2.0f, 0.0f });
		vec2 max = vec2_add(min, (vec2){ entity->texture->width, entity->texture->height });

		if (vec2_inside(pos, min, max))
			return entity;
	}

	return NULL;
}
