#include "SceneManager.h"

#include "Debugger.h"
#include "String/StringUtils.h"
#include "String/StringHash.h"

void SceneManager::AddScene(Scene* scene)
{
	Get().m_scenes[GenerateHash(scene->GetName().c_str())] = unique_ptr<Scene>(scene);

	if (Get().m_activeScene == nullptr)
		ChangeScene(scene->GetName());
}

void SceneManager::ChangeScene(const std::string& name)
{
	if (!stringCompare(Get().m_activeName, name))
	{
		Scene* s = GetScene(name);

		if (s != nullptr)
		{
			// Exit old Scene
			if (Get().m_activeScene != nullptr)
				Get().m_activeScene->OnExtit();

			Get().m_activeScene = s;
			Get().m_activeName = name;

			// Enter new scene
			Get().m_activeScene->OnEntry();
		}
	}
}

void SceneManager::OnEvent(Event& e)
{
	GetActiveScene()->OnEvent(e);
}

void SceneManager::OnUpdate()
{
	GetActiveScene()->OnUpdate();
}

void SceneManager::OnRender()
{
	GetActiveScene()->OnRender();
}

void SceneManager::OnRenderDebug()
{
	GetActiveScene()->OnRenderDebug();
}

void SceneManager::OnImGui()
{
	GetActiveScene()->OnImGui();
}

Scene* SceneManager::GetScene(const std::string& name)
{
	try
	{
		return Get().m_scenes.at(GenerateHash(name.c_str())).get();
	}
	catch (std::out_of_range)
	{
		DEBUG_WARN("No such scene: {0}", name);
		return nullptr;
	}
}

Scene* SceneManager::GetActiveScene()
{
	return Get().m_activeScene;
}
