#ifndef SCENE_H
#define SCENE_H

#include "ECS/Ecs.h"

#include "Camera/Camera.h"

#include "Application/EventHandler.h"
#include "Graphics/Background.h"

#include "clib/dynamic_array.h"

#define SCENE_INITIAL_LAYER_SIZE	8
#define SCENE_INITIAL_ECS_SIZE		8

CLIB_DYNAMIC_ARRAY_DECLARE_FUNCS(layer, EcsEntity)

typedef struct
{
	Camera* camera;

	float width;
	float height;

	World* world;

	Ecs ecs;

	clib_dynamic_array* layers;
	size_t max_layer;

	Background background;

	float smooth_movement;
} Scene;

int SceneLoad(Scene* scene, Camera* camera, float w, float h, size_t max_layer);
void SceneQuit(Scene* scene);

void SceneAddEntity(Scene* scene, EcsEntity* entity, size_t layer);
void SceneAddEntityPos(Scene* scene, EcsEntity* entity, size_t layer, vec2 position);
void SceneRemoveEntity(Scene* scene, const char* name, size_t layer);
void SceneClearEntities(Scene* scene);

void SceneOnEvent(Scene* scene, Event e);
void SceneOnUpdate(Scene* scene, float deltaTime);
void SceneOnRender(Scene* scene);
void SceneOnRenderDebug(Scene* scene);

EcsEntity* SceneGetEntity(Scene* scene, const char* name, size_t layer);
EcsEntity* SceneGetEntityAt(Scene* scene, vec2 position, size_t layer);

#endif /* !SCENE_H */