#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Application/Application.h"
#include "Graphics/Background.h"

#include "Frost/FrostEcs.h"

typedef struct 
{
	Ecs ecs;


	Camera* camera;

	/* active scene specific data */
	char name[APPLICATION_STR_LEN];
	float width;
	float height;

	Background background;

} Scenes;

int ScenesInit(Scenes* scenes, Camera* camera);
void ScenesDestroy(Scenes* scenes);

void ScenesClearActive(Scenes* scenes);

void ScenesOnEvent(Scenes* scenes, Event e);
void ScenesOnUpdate(Scenes* scenes, float deltatime);
void ScenesOnRender(Scenes* scenes);
void ScenesOnRenderDebug(Scenes* scenes);

#endif /* !SCENE_MANAGER_H */