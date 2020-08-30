#include "Scenes.h"

int ScenesInit(Scenes* scenes, Camera* camera)
{
	scenes->camera = camera;

	memset(scenes->name, '\0', APPLICATION_STR_LEN);
	scenes->width = 0.0f;
	scenes->height = 0.0f;

	AnimationConditionsInit();
	EcsInit(&scenes->ecs);

	return 1;
}

void ScenesDestroy(Scenes* scenes)
{
	if (scenes->name[0] != '\0')
		ScenesClearActive(scenes);

	AnimationConditionsDestroy();
	EcsDestroy(&scenes->ecs);
}

void ScenesClearActive(Scenes* scenes)
{
	BackgroundClear(&scenes->background);
	EcsClear(&scenes->ecs);

	memset(scenes->name, '\0', APPLICATION_STR_LEN);
	scenes->width = 0.0f;
	scenes->height = 0.0f;
}

void ScenesOnEvent(Scenes* scenes, Event e)
{

}

void ScenesOnUpdate(Scenes* scenes, float deltatime)
{
	BackgroundUpdate(&scenes->background, scenes->camera->position.x - scenes->camera->size.x / 2.0f, deltatime);

	EcsUpdate(&scenes->ecs, deltatime);
}

void ScenesOnRender(Scenes* scenes)
{
	BackgroundRender(&scenes->background, CameraGetViewProjectionPtr(scenes->camera));

	EcsRender(&scenes->ecs, CameraGetViewProjectionPtr(scenes->camera));
}

void ScenesOnRenderDebug(Scenes* scenes)
{

}
