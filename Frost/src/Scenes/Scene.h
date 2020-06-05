#ifndef SCENE_H
#define SCENE_H

#include "ECS/Ecs.h"

#include "Camera/Camera.h"

#include "Application/EventHandler.h"
#include "Graphics/Background.h"

#include "clib/dynamic_array.h"

#define SCENE_INITIAL_LAYER_SIZE	8
#define SCENE_INITIAL_ECS_SIZE		8

CLIB_DYNAMIC_ARRAY_DECLARE_FUNCS(entities, EcsEntity)

typedef struct
{
	Camera* camera;

	float width;
	float height;

	World* world;

	clib_dynamic_array entities;

	Background background;

	float smooth_movement;
} Scene;

int SceneLoad(Scene* scene, Camera* camera, float w, float h);
void SceneQuit(Scene* scene);

void SceneAddEntity(Scene* scene, EcsEntity* entit, int z_indexy);
void SceneAddEntityPos(Scene* scene, EcsEntity* entity, int z_index, vec2 position);
void SceneRemoveEntity(Scene* scene, const char* name);
void SceneClearEntities(Scene* scene);

void SceneOnEvent(Scene* scene, Event e);
void SceneOnUpdate(Scene* scene, Ecs* ecs, float deltaTime);
void SceneOnRender(Scene* scene, Ecs* ecs);
void SceneOnRenderDebug(Scene* scene);

EcsEntity* SceneGetEntity(Scene* scene, const char* name);
EcsEntity* SceneGetEntityAt(Scene* scene, vec2 position);

#endif /* !SCENE_H */