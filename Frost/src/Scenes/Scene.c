#include "Scene.h"

int SceneLoad(Scene* scene, Camera* camera, float w, float h)
{
	scene->camera = camera;
	scene->width = w;
	scene->height = h;

	return 1;
}

void SceneQuit(Scene* scene)
{
	BackgroundClear(&scene->background);
}

void SceneOnEvent(Scene* scene, Ecs* ecs, Event e)
{
}

void SceneOnUpdate(Scene* scene, Ecs* ecs, float deltaTime)
{
	BackgroundUpdate(&scene->background, scene->camera->position.x - scene->camera->size.x / 2.0f, deltaTime);

	EcsUpdate(ecs, deltaTime);
}

void SceneOnRender(Scene* scene, Ecs* ecs)
{
	BackgroundRender(&scene->background, CameraGetViewProjectionPtr(scene->camera));

	EcsRender(ecs, CameraGetViewProjectionPtr(scene->camera));
}

void SceneOnRenderDebug(Scene* scene, Ecs* ecs)
{

}