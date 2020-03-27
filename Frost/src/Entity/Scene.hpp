#pragma once

#include "Entity/Components.hpp"
#include "Event/EventHandler.hpp"

#include "Camera/Camera.hpp"

#include <map>

#include "clib/vector.h"

#define SCENE_INITIAL_LAYER_SIZE	8

CLIB_VECTOR_DECLARE_FUNCS(layer, Entity)

struct Scene
{
	Camera* camera;

	float width;
	float height;

	World* world;
	clib_vector* layers;
	size_t max_layer;

	float smooth_movement;
};

int SceneLoad(Scene* scene, Camera* camera, float w, float h, size_t max_layer);
void SceneQuit(Scene* scene);

void SceneAddEntity(Scene* scene, Entity* entity, size_t layer);
void SceneAddEntity(Scene* scene, Entity* entity, size_t layer, const glm::vec2& position);
void SceneRemoveEntity(Scene* scene, const std::string& name, size_t layer);
void SceneClearEntities(Scene* scene);

void SceneOnEvent(Scene* scene, const Event& e);
void SceneOnUpdate(Scene* scene, float deltaTime);
void SceneOnRender(Scene* scene);
void SceneOnRenderDebug(Scene* scene);

void SceneSetCameraPosition(Scene* scene, const glm::vec3& position);

Entity* SceneGetEntity(Scene* scene, const std::string& name, size_t layer);
Entity* SceneGetEntityAt(Scene* scene, const glm::vec2& pos, size_t layer);
clib_vector* SceneGetLayer(Scene* scene, size_t layer);

std::vector<size_t> SceneGetUsedLayers(Scene* scene);
