#include "Scene.h"

#include "ECS/Systems.h"

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

	clib_strmap_alloc(&scene->entity_templates, 16);

	return 1;
}

void SceneQuit(Scene* scene)
{
	BackgroundClear(&scene->background);

	clib_strmap_free(&scene->entity_templates);

	EcsDestroy(&scene->ecs);
	ComponentTableFree(&scene->components);
}

void SceneAddEntityTemplate(Scene* scene, const char* entity, const char* templ)
{
	clib_strmap_insert(&scene->entity_templates, entity, templ);
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

const char* SceneGetEntityAt(Scene* scene, vec2 pos)
{
	CLIB_DICT_ITERATE_FOR(&scene->components.table[COMPONENT_TRANSFORM], iter)
	{
		Transform* transform = clib_dict_iter_get_value(iter);

		vec2 min = vec2_sub(transform->position, (vec2) { transform->size.x / 2.0f, 0.0f });
		vec2 max = vec2_add(min, transform->size);

		if (vec2_inside(pos, min, max))
			return clib_dict_iter_get_key(iter);
	}

	return NULL;
}
