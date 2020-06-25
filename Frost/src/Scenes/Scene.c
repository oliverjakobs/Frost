#include "Scene.h"

#include "clib/hash.h"

#include "ECS/Systems.h"

#include "Application/Debugger.h"

int SceneLoad(Scene* scene, Camera* camera, float w, float h)
{
	scene->camera = camera;
	scene->width = w;
	scene->height = h;

	EcsInit(&scene->ecs, 4, 2);
	EcsAddUpdateSystem(&scene->ecs, PhysicsSystem);
	EcsAddUpdateSystem(&scene->ecs, PlayerSystem);
	EcsAddUpdateSystem(&scene->ecs, AnimationSystem);
	EcsAddUpdateSystem(&scene->ecs, InteractionSystem);
	EcsAddRenderSystem(&scene->ecs, RenderSystem);
	EcsAddRenderSystem(&scene->ecs, DebugRenderSystem);
		
	ComponentTableInit(&scene->components, 16);

	clib_hashset_alloc(&scene->entity_templates, clib_hash_int32, 16);

	return 1;
}

void SceneQuit(Scene* scene)
{
	BackgroundClear(&scene->background);

	clib_hashset_free(&scene->entity_templates);

	EcsDestroy(&scene->ecs);
	ComponentTableFree(&scene->components);
}

void SceneAddEntityTemplate(Scene* scene, EntityID entity, const char* templ)
{
	size_t size = strlen(templ);
	char* value = (char*)malloc(size + 1);

	if (!value)
	{
		DEBUG_WARN("[Scene] Failed to allocate memory for entity template (%s)\n", templ);
		return;
	}

	strcpy(value, templ);
	value[size] = '\0'; /* make sure key is null-terminated */

	if ((char*)clib_hashset_insert(&scene->entity_templates, entity, (void*)value) != value)
	{
		DEBUG_WARN("[Scene] Failed to add entity template (%s)\n", templ);
		free(value);
	}
}

void SceneClearEntityTemplates(Scene* scene)
{
	CLIB_HASHSET_ITERATE_FOR(&scene->entity_templates, iter)
	{
		free(clib_hashset_iter_get_value(iter));
	}
	clib_hashset_clear(&scene->entity_templates);
}

void SceneOnEvent(Scene* scene, Event e)
{
}

void SceneOnUpdate(Scene* scene, float deltaTime)
{
	BackgroundUpdate(&scene->background, scene->camera->position.x - scene->camera->size.x / 2.0f, deltaTime);

	EcsUpdate(&scene->ecs, &scene->components, deltaTime);
}

void SceneOnRender(Scene* scene)
{
	BackgroundRender(&scene->background, CameraGetViewProjectionPtr(scene->camera));

	EcsRender(&scene->ecs, &scene->components, CameraGetViewProjectionPtr(scene->camera));
}

void SceneOnRenderDebug(Scene* scene)
{

}