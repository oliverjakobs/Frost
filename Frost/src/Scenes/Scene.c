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

	return 1;
}

void SceneQuit(Scene* scene)
{
	BackgroundClear(&scene->background);

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