#pragma once

#include "Entity/Scene.h"

using namespace ignis;

class SceneManager
{
private:
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<Scene> m_scene;

	// editor
	bool m_editmode;
	bool m_showgrid;

	float m_gridsize;
	float m_padding;

	std::shared_ptr<Entity> m_active;
public:
	SceneManager(std::shared_ptr<Camera> camera);

	void LoadScene(const std::string& path);

	void OnEvent(const Event& e);
	void OnUpdate(float deltaTime);
	void OnRender();
	void OnRenderDebug();
	void OnImGui();

	glm::vec2 GetMousePos() const;

	Scene* GetScene() const;
};