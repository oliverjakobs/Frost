#pragma once

#include "Scene.h"

class SceneManager
{
private:
	std::map<std::string, std::string> m_register;

	unique_ptr<Scene> m_scene;
	std::string m_sceneName;
public:
	static Scene* CreateSceneFromFile(const std::string& name, const std::string& path);

	void RegisterScene(const std::string& name, const std::string& path);
	Scene* LoadScene(const std::string& name);
	void ChangeScene(const std::string& name);

	// Functions for scene
	void OnUpdate();
	void OnRender();
	void OnRenderDebug();
	void OnImGui();

	Scene* GetScene() const;
};