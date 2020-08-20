#ifndef SCENE_H
#define SCENE_H

#include "ECS/Systems.h"

#include "Camera/Camera.h"

#include "Application/Application.h"
#include "Graphics/Background.h"

typedef struct
{
	Camera* camera;

	float width;
	float height;

	Background background;
} Scene;

int SceneLoad(Scene* scene, Camera* camera, float w, float h);
void SceneQuit(Scene* scene);

void SceneOnEvent(Scene* scene, Ecs* ecs, Event e);
void SceneOnUpdate(Scene* scene, Ecs* ecs, float deltaTime);
void SceneOnRender(Scene* scene, Ecs* ecs);
void SceneOnRenderDebug(Scene* scene, Ecs* ecs);

#endif /* !SCENE_H */