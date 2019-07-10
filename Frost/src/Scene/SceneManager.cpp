#include "SceneManager.h"

#include "Debugger.h"
#include "String/StringUtils.h"

#include "Event/ApplicationEvent.h"

#include "Script/JSONParser.h"

Scene* SceneManager::CreateSceneFromFile(const std::string& name, const std::string& path)
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

	//DEBUG_INFO("[JSON] Loaded scene {0} from {1}", name, path);

	return scene;
}

void SceneManager::RegisterScene(const std::string& name, const std::string& path)
{
	m_register.insert({ name, path });
}

Scene* SceneManager::LoadScene(const std::string& name)
{
	try
	{
		return CreateSceneFromFile(name, m_register.at(name));
	}
	catch (std::out_of_range)
	{
		DEBUG_WARN("No such scene: {0}", name);
		return nullptr;
	}
}

void SceneManager::ChangeScene(const std::string& name)
{
	if (!stringCompare(m_sceneName, name))
	{
		Scene* newScene = LoadScene(name);

		if (newScene != nullptr)
		{
			m_scene = unique_ptr<Scene>(newScene);
			m_sceneName = newScene->GetName();
		}
	}
}

void SceneManager::OnEvent(Event& e)
{
	if (e.GetEventType() == EventType::ChangeScene)
	{
		ChangeSceneEvent& change = (ChangeSceneEvent&)e;
		ChangeScene(change.GetTarget());
	}
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

Scene* SceneManager::GetScene() const
{
	return m_scene.get();	
}

