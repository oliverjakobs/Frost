#ifndef SCENE_H
#define SCENE_H

#include "ECS/Ecs.h"
#include "ECS/Systems.h"

#include "Camera/Camera.h"

#include "Application/Application.h"
#include "Graphics/Background.h"

#define SCENE_INITIAL_LAYER_SIZE	8
#define SCENE_INITIAL_ECS_SIZE		8

typedef struct
{
	Camera* camera;

	float width;
	float height;

	Background background;

	Ecs ecs;

	clib_array entity_templates;
} Scene;

int SceneLoad(Scene* scene, Camera* camera, float w, float h);
void SceneQuit(Scene* scene);

void SceneOnEvent(Scene* scene, Event e);
void SceneOnUpdate(Scene* scene, float deltaTime);
void SceneOnRender(Scene* scene);
void SceneOnRenderDebug(Scene* scene);

typedef struct
{
	EntityID entity;
	char* templ;
} EntityTemplate;

void SceneAddEntityTemplate(Scene* scene, EntityID entity, const char* templ);
void SceneClearEntityTemplates(Scene* scene);

#endif /* !SCENE_H */