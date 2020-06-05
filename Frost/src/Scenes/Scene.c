#include "Scene.h"

CLIB_DYNAMIC_ARRAY_DEFINE_FUNCS(entities, EcsEntity)

int SceneLoad(Scene* scene, Camera* camera, float w, float h)
{
	clib_dynamic_array_init(&scene->entities, 0);

	scene->camera = camera;
	scene->width = w;
	scene->height = h;

	scene->world = (World*)malloc(sizeof(World));
	WorldLoad(scene->world, (vec2){ 0.0f, -980.0f });

	scene->smooth_movement = 0.5f;

	EcsComponentListInit(&scene->components, 16);

	return 1;
}

void SceneQuit(Scene* scene)
{
	BackgroundClear(&scene->background);

	SceneClearEntities(scene);
	clib_dynamic_array_free(&scene->entities);

	WorldDestroy(scene->world);
	free(scene->world);

	EcsComponentListDelete(&scene->components);
}

int _SceneEntityCmp(const EcsEntity** a, const EcsEntity** b)
{
	return (*a)->z_index - (*b)->z_index;
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
	entities_dynamic_array_push(&scene->entities, entity);

	/* sort by z_index */
	qsort((EcsEntity**)scene->entities.elements, scene->entities.size, sizeof(EcsEntity*), _SceneEntityCmp);
}

void SceneAddEntityPos(Scene* scene, EcsEntity* entity, int z_index, vec2 position)
{
	if (!entity) return;

	EcsEntitySetPosition(entity, position);
	SceneAddEntity(scene, entity, z_index);
}

void SceneRemoveEntity(Scene* scene, const char* name)
{
	for (size_t i = 0; i < scene->entities.size; i++)
	{
		if (strcmp(entities_dynamic_array_get(&scene->entities, i)->name, name) == 0)
		{
			EcsEntity* e = entities_dynamic_array_get(&scene->entities, i);
			entities_dynamic_array_delete(&scene->entities, i);
			free(e);
			return;
		}
	}
}

void SceneClearEntities(Scene* scene)
{
	for (size_t i = 0; i < scene->entities.size; i++)
	{
		EcsEntity* e = entities_dynamic_array_get(&scene->entities, i);
		entities_dynamic_array_delete(&scene->entities, i);
		free(e);
	}
}

void SceneOnEvent(Scene* scene, Event e)
{
}

void SceneOnUpdate(Scene* scene, Ecs* ecs, float deltaTime)
{
	WorldTick(scene->world, deltaTime);

	BackgroundUpdate(&scene->background, scene->camera->position.x - scene->camera->size.x / 2.0f, deltaTime);

	EcsUpdate(ecs, (EcsEntity**)scene->entities.elements, scene->entities.size, deltaTime);
}

void SceneOnRender(Scene* scene, Ecs* ecs)
{
	BackgroundRender(&scene->background, CameraGetViewProjectionPtr(scene->camera));

	EcsRender(ecs, (EcsEntity**)scene->entities.elements, scene->entities.size, CameraGetViewProjectionPtr(scene->camera));
}

void SceneOnRenderDebug(Scene* scene)
{

}

EcsEntity* SceneGetEntity(Scene* scene, const char* name)
{
	for (size_t i = 0; i < scene->entities.size; ++i)
	{
		EcsEntity* entity = entities_dynamic_array_get(&scene->entities, i);

		if (strcmp(entity->name, name) == 0)
			return entity;
	}

	return NULL;
}

EcsEntity* SceneGetEntityAt(Scene* scene, vec2 pos)
{
	for (size_t i = 0; i < scene->entities.size; ++i)
	{
		EcsTextureComponent* tex = entities_dynamic_array_get(&scene->entities, i)->texture;

		if (!tex) continue;

		vec2 position = EcsEntityGetPosition(entities_dynamic_array_get(&scene->entities, i));

		vec2 min = vec2_sub(position, (vec2){ tex->width / 2.0f, 0.0f });
		vec2 max = vec2_add(min, (vec2){ tex->width, tex->height });

		if (vec2_inside(pos, min, max))
			return entities_dynamic_array_get(&scene->entities, i);
	}

	return NULL;
}
