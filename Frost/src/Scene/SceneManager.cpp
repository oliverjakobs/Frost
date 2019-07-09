#include "SceneManager.h"

#include "Debugger.h"
#include "String/StringUtils.h"

#include "Script/JSONParser.h"

void SceneManager::Load(const std::string& path)
{
	DEBUG_INFO("Loading scenes from {0}", path);

	json root = jsonParseFile(path);

	for (auto&[sceneName, sceneJson] : root.items())
	{
		std::string map = jsonToString(sceneJson, "map");

		Scene* scene = new Scene(sceneName, new TileMap(map));
		AddScene(scene);

		for (auto&[entityName, entityJson] : sceneJson.at("entities").items())
		{
			scene->AddEntity(entityName, jsonToString(entityJson, "scr"));

			if (entityJson.find("position") != entityJson.end())
			{
				scene->AddSpawnPosition(entityName, jsonToVec2(entityJson, "position"));
			}
		}

		DEBUG_INFO("Scene {0} loaded", sceneName);
	}
}

void SceneManager::AddScene(Scene* scene)
{
	Get().m_scenes.insert({ scene->GetName(), unique_ptr<Scene>(scene) });

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
		return Get().m_scenes.at(name).get();
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
