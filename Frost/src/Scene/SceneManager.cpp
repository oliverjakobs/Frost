#include "SceneManager.h"

#include "Debugger.h"
#include "String/StringUtils.h"

#include "Script/JSONParser.h"

void SceneManager::RegisterScene(const std::string& name, const std::string& path)
{
	Get().m_register.insert({ name, path });
}

Scene* SceneManager::LoadScene(const std::string& name)
{
	try
	{
		return LoadSceneFromFile(name, Get().m_register.at(name));
	}
	catch (std::out_of_range)
	{
		DEBUG_WARN("No such scene: {0}", name);
		return nullptr;
	}
}

Scene* SceneManager::LoadSceneFromFile(const std::string& name, const std::string& path)
{
	Scene* scene = nullptr;

	json root = jsonParseFile(path);

	std::string map = jsonToString(root, "map");

	if (map.empty())
	{
		DEBUG_WARN("[JSON] Could not load scene {0}: Map is empty ({1})", name, path);
		return scene;
	}

	scene = new Scene(name, new TileMap(map));

	for (auto&[entityName, entityJson] : root.at("entities").items())
	{
		std::string entitySrc = jsonToString(entityJson, "scr");

		if (entitySrc.empty())
		{
			DEBUG_WARN("[JSON] Entity {0} has no src ({1})", entityName, path);
			continue;
		}

		Entity* entity = EntityManager::CreateEntity(scene, entitySrc);

		if (entity != nullptr)
		{
			if (entityJson.find("position") != entityJson.end())
			{
				entity->SetPosition(jsonToVec2(entityJson, "position"));
			}

			scene->AddEntity(entityName, entity);
		}
	}

	DEBUG_INFO("[JSON] Loaded scene {0} from {1}", name, path);

	return scene;
}

void SceneManager::ChangeScene(const std::string& name)
{
	if (!stringCompare(Get().m_sceneName, name))
	{
		Scene* newScene = LoadScene(name);

		if (newScene != nullptr)
		{
			// Exit old Scene
			if (Get().m_scene)
				Get().m_scene->OnExtit();

			Get().m_scene = unique_ptr<Scene>(newScene);
			Get().m_sceneName = newScene->GetName();

			// Enter new scene
			Get().m_scene->OnEntry();
		}
	}
}

void SceneManager::OnEvent(Event& e)
{
	GetScene()->OnEvent(e);
}

void SceneManager::OnUpdate()
{
	GetScene()->OnUpdate();
}

void SceneManager::OnRender()
{
	GetScene()->OnRender();
}

void SceneManager::OnRenderDebug()
{
	GetScene()->OnRenderDebug();
}

void SceneManager::OnImGui()
{
	GetScene()->OnImGui();
}

Scene* SceneManager::GetScene()
{
	return Get().m_scene.get();	
}

