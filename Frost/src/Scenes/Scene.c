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

	EcsInit(&scene->ecs, 3, 2);
	EcsAddUpdateSystem(&scene->ecs, EcsSystemPlayer);
	EcsAddUpdateSystem(&scene->ecs, EcsSystemAnimation);
	EcsAddUpdateSystem(&scene->ecs, EcsSystemInteraction);
	EcsAddRenderSystem(&scene->ecs, EcsSystemRender, EcsSystemRenderPre, EcsSystemRenderPost);
	EcsAddRenderSystem(&scene->ecs, EcsSystemDebugRender, EcsSystemDebugRenderPre, EcsSystemDebugRenderPost);
	
	clib_array_alloc(&scene->entities, 16, sizeof(EcsEntity));
	ComponentTableInit(&scene->components, 16);

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
	ComponentTableFree(&scene->components);
}

int _SceneEntityCmp(const EcsEntity* a, const EcsEntity* b)
{
	Transform* transform_a = ComponentTableGetComponent(a->components, a->name, COMPONENT_TRANSFORM);
	Transform* transform_b = ComponentTableGetComponent(b->components, b->name, COMPONENT_TRANSFORM);

	if (transform_a && transform_b)
		return transform_a->z_index - transform_b->z_index;

	return 0;
}

void SceneAddEntity(Scene* scene, EcsEntity* entity, int z_index)
{
	if (!entity)
		return;

	EcsPhysicsComponent* phys = ComponentTableGetComponent(&scene->components, entity->name, COMPONENT_PHYSICS);
	if (phys)
	{
		WorldAddBody(scene->world, &phys->body);
	}

	EcsCameraComponent* cam = ComponentTableGetComponent(&scene->components, entity->name, COMPONENT_CAMERA);
	if (cam)
	{
		cam->camera = scene->camera;
		cam->scene_w = scene->width;
		cam->scene_h = scene->height;
	}

	Transform* trans = ComponentTableGetComponent(&scene->components, entity->name, COMPONENT_TRANSFORM);
	if (trans)
	{
		trans->z_index = z_index;
	}

	clib_array_push(&scene->entities, entity);

	/* sort by z_index */
	clib_array_sort(&scene->entities, _SceneEntityCmp);
}

void SceneAddEntityPos(Scene* scene, EcsEntity* entity, int z_index, vec2 position)
{
	if (!entity) return;

	ComponentTableSetEntityPosition(&scene->components, entity->name, position);
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
	ComponentTableClear(&scene->components);
	clib_array_clear(&scene->entities);
}

void SceneOnEvent(Scene* scene, Event e)
{
}

void SceneOnUpdate(Scene* scene, float deltaTime)
{
	WorldTick(scene->world, deltaTime);

	BackgroundUpdate(&scene->background, scene->camera->position.x - scene->camera->size.x / 2.0f, deltaTime);

	EcsUpdate(&scene->ecs, (EcsEntity*)scene->entities.data, scene->entities.used, &scene->components, deltaTime);
}

void SceneOnRender(Scene* scene)
{
	BackgroundRender(&scene->background, CameraGetViewProjectionPtr(scene->camera));

	EcsRender(&scene->ecs, (EcsEntity*)scene->entities.data, scene->entities.used, &scene->components, CameraGetViewProjectionPtr(scene->camera));
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

		EcsTextureComponent* texture = ComponentTableGetComponent(&scene->components, entity->name, COMPONENT_TEXTURE);

		if (!texture) continue;

		vec2 position = ComponentTableGetEntityPosition(&scene->components, entity->name);

		vec2 min = vec2_sub(position, (vec2){ texture->width / 2.0f, 0.0f });
		vec2 max = vec2_add(min, (vec2){ texture->width, texture->height });

		if (vec2_inside(pos, min, max))
			return entity;
	}

	return NULL;
}
