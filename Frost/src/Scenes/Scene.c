#include "Scene.h"

#include "clib/hash.h"

#include "ECS/Systems.h"

#include "Application/Debugger.h"

int SceneLoad(Scene* scene, Camera* camera, float w, float h)
{
	scene->camera = camera;
	scene->width = w;
	scene->height = h;

	EcsInit(&scene->ecs, 4, 2, 8);
	EcsAddUpdateSystem(&scene->ecs, PhysicsSystem);
	EcsAddUpdateSystem(&scene->ecs, PlayerSystem);
	EcsAddUpdateSystem(&scene->ecs, AnimationSystem);
	EcsAddUpdateSystem(&scene->ecs, InteractionSystem);
	EcsAddRenderSystem(&scene->ecs, RenderSystem);
	EcsAddRenderSystem(&scene->ecs, DebugRenderSystem);
		
	EcsRegisterDataComponent(&scene->ecs, sizeof(Transform), NULL);
	EcsRegisterDataComponent(&scene->ecs, sizeof(RigidBody), NULL);
	EcsRegisterDataComponent(&scene->ecs, sizeof(Movement), NULL);
	EcsRegisterDataComponent(&scene->ecs, sizeof(Sprite), NULL);
	EcsRegisterDataComponent(&scene->ecs, sizeof(Animator), AnimatorFree);
	EcsRegisterDataComponent(&scene->ecs, sizeof(CameraController), NULL);
	EcsRegisterDataComponent(&scene->ecs, sizeof(Interaction), NULL);
	EcsRegisterDataComponent(&scene->ecs, sizeof(Interactor), NULL);

	EcsRegisterOrderComponent(&scene->ecs, sizeof(Template), TemplateCmp);
	EcsRegisterOrderComponent(&scene->ecs, sizeof(ZIndex), ZIndexCmp);

	clib_array_alloc(&scene->entity_templates, 16, sizeof(EntityTemplate));

	return 1;
}

void SceneQuit(Scene* scene)
{
	BackgroundClear(&scene->background);

	clib_array_free(&scene->entity_templates);

	EcsDestroy(&scene->ecs);
}

void SceneOnEvent(Scene* scene, Event e)
{
}

void SceneOnUpdate(Scene* scene, float deltaTime)
{
	BackgroundUpdate(&scene->background, scene->camera->position.x - scene->camera->size.x / 2.0f, deltaTime);

	EcsUpdate(&scene->ecs, deltaTime);
}

void SceneOnRender(Scene* scene)
{
	BackgroundRender(&scene->background, CameraGetViewProjectionPtr(scene->camera));

	EcsRender(&scene->ecs, CameraGetViewProjectionPtr(scene->camera));
}

void SceneOnRenderDebug(Scene* scene)
{

}

void SceneAddEntityTemplate(Scene* scene, EntityID entity, const char* templ)
{
	EntityTemplate t;
	t.entity = entity;

	size_t size = strlen(templ);
	t.templ = (char*)malloc(size);

	if (!t.templ)
	{
		DEBUG_WARN("[Scene] Failed to allocate memory for entity template (%s)\n", templ);
		return;
	}

	strcpy(t.templ, templ);

	clib_array_push(&scene->entity_templates, &t);
}

void SceneClearEntityTemplates(Scene* scene)
{
	for (size_t i = 0; i < scene->entity_templates.used; ++i)
	{
		free(((EntityTemplate*)clib_array_get(&scene->entity_templates, i))->templ);
	}
	clib_array_free(&scene->entity_templates);
}