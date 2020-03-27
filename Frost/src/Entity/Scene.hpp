#pragma once

#include "Entity/Components.hpp"
#include "Event/EventHandler.hpp"

#include "Camera/Camera.hpp"

#include <map>

#define SCENE_MAX_LAYER	8

struct SceneLayer
{
	Entity** entities;
	size_t used;
	size_t size;
};

struct Scene
{
	Camera* camera;

	float width;
	float height;

	std::unique_ptr<World> world;
	std::vector<Entity*> entities[SCENE_MAX_LAYER];

	float smooth_movement;
};

int SceneLoad(Scene* scene, Camera* camera, float w, float h);
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
std::vector<Entity*> SceneGetEntities(Scene* scene, size_t layer);

std::vector<size_t> SceneGetUsedLayers(Scene* scene);
