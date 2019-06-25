#pragma once

#include "Scene.h"

#include <unordered_map>

class SceneManager : private Singleton<SceneManager>
{
private:
	std::unordered_map<uint32_t, unique_ptr<Scene>> m_scenes;

	Scene* m_activeScene;
	std::string m_activeName;
public:
	static void AddScene(Scene* scene);
	static void ChangeScene(const std::string& name);

	static void OnEvent(Event& e);
	static void OnUpdate();
	static void OnRender();
	static void OnRenderDebug();

	static Scene* GetScene(const std::string& name);
	static Scene* GetActiveScene();
};