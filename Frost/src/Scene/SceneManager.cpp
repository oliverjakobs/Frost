#include "SceneManager.h"

void SceneManager::AddScene(const std::string& name, Scene* scene)
{
	Get().m_scenes[name] = unique_ptr<Scene>(scene);

	if (Get().m_activeScene == nullptr)
		ChangeScene(name);
}

void SceneManager::ChangeScene(const std::string& name)
{
	if (!stringCompare(Get().m_activeName, name))
	{
		Scene* s = GetScene(name);

		if (s != nullptr)
		{
			Get().m_activeScene = s;
			Get().m_activeName = name;
		}
	}
}

void SceneManager::OnInput()
{
	Get().m_activeScene->onInput();
}

void SceneManager::OnUpdate()
{
	Get().m_activeScene->onUpdate();
}

void SceneManager::OnRender()
{
	Get().m_activeScene->onRender();
}

void SceneManager::OnRenderDebug()
{
	Get().m_activeScene->onRenderDebug();
}

Scene* SceneManager::GetScene(const std::string& name)
{
	try
	{
		return Get().m_scenes.at(name).get();
	}
	catch (std::out_of_range)
	{
		DEBUG_MESSAGE("No such scene: " << name);
		return nullptr;
	}
}

Scene* SceneManager::GetActiveScene()
{
	return Get().m_activeScene;
}
