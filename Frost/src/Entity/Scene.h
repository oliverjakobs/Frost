#pragma once

#include "Entity/Components.h"
#include "Event/EventHandler.h"

class Scene
{
private:
	std::shared_ptr<ignis::Camera> m_camera;

	std::vector<std::shared_ptr<Entity>> m_entities;

	float m_smoothMovement;
public:
	Scene(std::shared_ptr<ignis::Camera> camera);
	~Scene();

	void AddEntity(std::shared_ptr<Entity> entity);
	void AddEntity(std::shared_ptr<Entity> entity, const glm::vec2& position);
	void RemoveEntity(const std::string& name);
	void Clear();

	void OnEvent(Event& e);
	void OnUpdate(float deltaTime);
	void OnRender();
	void OnRenderDebug();

	void SetCameraPosition(const glm::vec3& position);

	std::shared_ptr<Entity> GetEntity(const std::string& name) const;
	std::vector<std::shared_ptr<Entity>> GetEntities() const { return m_entities; };

	ignis::Camera* GetCamera() const { return m_camera.get(); }

	const float GetWidth() const { return 0.0f; }
	const float GetHeight() const { return 0.0f; }
};