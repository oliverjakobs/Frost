#pragma once

#include "Scene.h"

class SceneManager : private Singleton<SceneManager>
{
private:
	std::map<std::string, Scene*> m_scenes;

	Scene* m_activeScene;
	std::string m_activeName;
public:
	static void AddScene(const std::string& name, Scene* scene);
	static void ChangeScene(const std::string& name);

	static void OnInput();
	static void OnUpdate();
	static void OnRender();
	static void OnRenderDebug();

	static Scene* GetScene(const std::string& name);
};