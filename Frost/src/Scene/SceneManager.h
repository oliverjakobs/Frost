#pragma once

#include "Scene.h"

#include <unordered_map>

class SceneManager : private Singleton<SceneManager>
{
private:
	std::map<std::string, std::string> m_register;

	unique_ptr<Scene> m_scene;
	std::string m_sceneName;
public:
	static void RegisterScene(const std::string& name, const std::string& path);

	static Scene* LoadScene(const std::string& name);
	static Scene* LoadSceneFromFile(const std::string& name, const std::string& path);
	static void ChangeScene(const std::string& name);

	// Functions for scene
	static void OnEvent(Event& e);
	static void OnUpdate();
	static void OnRender();
	static void OnRenderDebug();
	static void OnImGui();

	static Scene* GetScene();
};