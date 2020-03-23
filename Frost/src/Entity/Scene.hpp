#pragma once

#include "Entity/Components.hpp"
#include "Event/EventHandler.hpp"

#include "Camera/OrthographicCamera.hpp"

#include <map>

class Scene
{
private:
	std::shared_ptr<Camera> m_camera;

	float m_width;
	float m_height;

	std::unique_ptr<World> m_world;
	std::map<size_t, std::vector<std::shared_ptr<Entity>>> m_entities;

	float m_smoothMovement;
public:
	Scene(std::shared_ptr<Camera> camera, float w, float h);
	~Scene();

	void AddEntity(std::shared_ptr<Entity> entity, size_t layer);
	void AddEntity(std::shared_ptr<Entity> entity, size_t layer, const glm::vec2& position);
	void RemoveEntity(const std::string& name, size_t layer);
	void Clear();

	void OnEntry();
	void OnExtit();

	void OnEvent(const Event& e);
	void OnUpdate(float deltaTime);
	void OnRender();
	void OnRenderDebug();

	void SetCameraPosition(const glm::vec3& position);

	std::shared_ptr<Entity> GetEntity(const std::string& name, size_t layer) const;
	std::shared_ptr<Entity> GetEntityAt(const glm::vec2& pos, size_t layer) const;
	std::vector<std::shared_ptr<Entity>> GetEntities(size_t layer) const;

	std::vector<size_t> GetLayers() const;

	World* GetWorld() const { return m_world.get(); }
	Camera* GetCamera() const { return m_camera.get(); }

	const float GetWidth() const { return m_width; }
	const float GetHeight() const { return m_height; }
};