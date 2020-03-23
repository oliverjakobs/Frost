#pragma once

#include "Entity/Scene.hpp"

#include <map>

class SceneManager
{
private:
	std::shared_ptr<Camera> m_camera;

	std::map<std::string, std::string> m_register;

	std::shared_ptr<Scene> m_scene;
	std::string m_sceneName;

	// editor
	bool m_editmode;
	bool m_showgrid;

	float m_gridsize;
	float m_padding;

	std::shared_ptr<Entity> m_active;
public:
	SceneManager(std::shared_ptr<Camera> camera, float gridsize, uint16_t padding);
	~SceneManager();

	void RegisterScenes(const std::string& path);

	void RegisterScene(const std::string& name, const std::string& path);
	void ChangeScene(const std::string& name);

	std::shared_ptr<Scene> LoadScene(const std::string& name);

	void OnEvent(const Event& e);
	void OnUpdate(float deltaTime);
	void OnRender();
	void OnRenderDebug();
	void OnImGui();

	glm::vec2 GetMousePos() const;

	Scene* GetScene() const { return m_scene.get(); }
	std::string GetSceneName() const { return m_sceneName; }

	std::shared_ptr<Camera> GetCamera() const { return m_camera; }
};