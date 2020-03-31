#pragma once

#include "ECS/Entity.h"

#include "Camera/Camera.h"

#include "Event/EventHandler.h"
#include "Background.h"

#include "clib/vector.h"

#define SCENE_INITIAL_LAYER_SIZE	8

CLIB_VECTOR_DECLARE_FUNCS(layer, EcsEntity)

struct Scene
{
	Camera* camera;

	float width;
	float height;

	World* world;

	clib_vector* layers;
	size_t max_layer;

	Background background;

	float smooth_movement;
};

int SceneLoad(Scene* scene, Camera* camera, float w, float h, size_t max_layer);
void SceneQuit(Scene* scene);

void SceneAddEntity(Scene* scene, EcsEntity* entity, size_t layer);
void SceneAddEntity(Scene* scene, EcsEntity* entity, size_t layer, const vec2 position);
void SceneRemoveEntity(Scene* scene, const char* name, size_t layer);
void SceneClearEntities(Scene* scene);

void SceneOnEvent(Scene* scene, const Event e);
void SceneOnUpdate(Scene* scene, float deltaTime);
void SceneOnRender(Scene* scene);
void SceneOnRenderDebug(Scene* scene);

void SceneSetCameraPosition(Scene* scene, const vec2 position);

EcsEntity* SceneGetEntity(Scene* scene, const char* name, size_t layer);
EcsEntity* SceneGetEntityAt(Scene* scene, const vec2 pos, size_t layer);