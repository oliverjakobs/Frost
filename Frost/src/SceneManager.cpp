#include "SceneManager.hpp"

#include "json/TemplateLoader.hpp"
#include "json/tb_json.h"

#include "Application.hpp"

#define SCENE_MANAGER_NAMELEN	32
#define SCENE_MANAGER_PATHLEN	64

int SceneManagerInit(SceneManager* manager, ResourceManager* resources, Camera* camera, float gridsize, uint16_t padding)
{
	manager->resources = resources;
	manager->camera = camera;

	manager->editmode = true;
	manager->showgrid = false;
	manager->layer = 0;

	manager->gridsize = gridsize;
	manager->padding = gridsize * padding;

	manager->hover = nullptr;

	return 1;
}

void SceneManagerDelete(SceneManager* manager)
{
	if (manager->scene)
		SceneQuit(manager->scene.get());
}

void SceneManagerRegisterScenes(SceneManager* manager, const char* json_path)
{
	char* json = ignisReadFile(json_path, NULL);

	if (!json)
	{
		OBELISK_WARN("Register not found (%s)", json_path);
		return;
	}

	tb_json_element scenes;
	tb_json_read(json, &scenes, "{'scenes'");

	for (int i = 0; i < scenes.elements; i++)
	{
		char name[SCENE_MANAGER_NAMELEN];
		tb_json_string((char*)scenes.value, "{*", name, SCENE_MANAGER_NAMELEN, &i);

		tb_json_element scene;
		tb_json_read_format((char*)scenes.value, &scene, "{'%s'", name);

		char path[SCENE_MANAGER_PATHLEN];
		strncpy(path, (char*)scene.value, scene.bytelen);

		path[scene.bytelen] = '\0';

		SceneManagerRegisterScene(manager, name, path);
	}

	free(json);
}

void SceneManagerRegisterScene(SceneManager* manager, const std::string& name, const std::string& path)
{
	if (!(name.empty() || path.empty()))
	{
		char* json = ignisReadFile(path.c_str(), NULL);

		if (json)
			manager->scenes.insert({ name, json });

		free(json);
	}
}

void SceneManagerChangeScene(SceneManager* manager, const std::string& name)
{
	if (!obelisk::StringCompare(manager->sceneName, name))
	{
		auto newScene = SceneManagerLoadScene(manager, name);

		if (newScene)
		{
			// Exit old Scene
			if (manager->scene)
				SceneQuit(manager->scene.get());

			manager->scene = newScene;
			manager->sceneName = name;

			// Enter new scene
			// SceneOnEntry(manager->scene.get());
		}
		else
		{
			OBELISK_WARN("Failed to load scene: %s", name.c_str());
		}
	}
}

std::shared_ptr<Scene> SceneManagerLoadScene(SceneManager* manager, const std::string& name)
{
	/* Check if scene is in the register */
	auto path = manager->scenes.find(name);

	if (path == manager->scenes.end())
	{
		OBELISK_WARN("Couldn't find scene: ", name.c_str());
		return nullptr;
	}

	char* json = (char*)path->second.c_str();

	float width = tb_json_float(json, "{'size'[0", NULL);
	float height = tb_json_float(json, "{'size'[1", NULL);

	auto scene = std::make_shared<Scene>();
	SceneLoad(scene.get(), manager->camera, width, height);

	tb_json_element templates;
	tb_json_read(json, &templates, "{'templates'");

	if (templates.error == TB_JSON_OK && templates.data_type == TB_JSON_ARRAY)
	{
		char* value = (char*)templates.value;

		for (int i = 0; i < templates.elements; i++)
		{
			tb_json_element entity;
			value = tb_json_array_step(value, &entity);

			char path[SCENE_MANAGER_PATHLEN];
			tb_json_string((char*)entity.value, "[0", path, SCENE_MANAGER_PATHLEN, NULL);

			float x = tb_json_float((char*)entity.value, "[1[0", NULL);
			float y = tb_json_float((char*)entity.value, "[1[1", NULL);

			int layer = tb_json_int((char*)entity.value, "[2", NULL);

			SceneAddEntity(scene.get(), TemplateLoadEntity(path, manager->resources), layer, glm::vec2(x, y));
		}
	}

	return scene;
}

void SceneManagerOnEvent(SceneManager* manager, const Event& e)
{
	if (e.Type == EventType::WindowResize)
	{
		WindowResizeEvent& resize = (WindowResizeEvent&)e;
		CameraSetProjectionOrtho(manager->camera, (float)resize.Width, (float)resize.Height);
	}

	if (e.Type == EventType::KeyPressed)
	{
		switch (((KeyPressedEvent&)e).KeyCode)
		{
		case KEY_F1:
			manager->editmode = !manager->editmode;
			break;
		case KEY_DELETE:
			SceneRemoveEntity(manager->scene.get(), "tree", 0);
			break;
		}
	}

	SceneOnEvent(manager->scene.get(), e);
}

void SceneManagerOnUpdate(SceneManager* manager, float deltaTime)
{
	if (!manager->editmode)
	{
		SceneOnUpdate(manager->scene.get(), deltaTime);
	}
	else
	{
		float cameraspeed = 400.0f;
		glm::vec3 position = manager->camera->position;

		if (InputKeyPressed(KEY_A))
			position.x -= cameraspeed * deltaTime;
		if (InputKeyPressed(KEY_D))
			position.x += cameraspeed * deltaTime;
		if (InputKeyPressed(KEY_S))
			position.y -= cameraspeed * deltaTime;
		if (InputKeyPressed(KEY_W))
			position.y += cameraspeed * deltaTime;

		manager->camera->position = position;
		CameraUpdateViewOrtho(manager->camera);

		manager->hover = SceneGetEntityAt(manager->scene.get(), CameraGetMousePos(manager->camera, InputMousePosition()), manager->layer);
	}
}

void SceneManagerOnRender(SceneManager* manager)
{
	SceneOnRender(manager->scene.get());

	if (manager->editmode)
	{
		// render grid
		Primitives2DStart(CameraGetViewProjectionPtr(manager->camera));

		if (manager->showgrid)
		{
			IgnisColorRGBA color = IGNIS_WHITE;
			ignisBlendColorRGBA(&color, 0.2f);

			for (float x = -manager->padding; x <= manager->scene->width + manager->padding; x += manager->gridsize)
				Primitives2DRenderLine(x, -manager->padding, x, manager->scene->height + manager->padding, color);

			for (float y = -manager->padding; y <= manager->scene->height + manager->padding; y += manager->gridsize)
				Primitives2DRenderLine(-manager->padding, y, manager->scene->width + manager->padding, y, color);
		}

		for (auto& entity : SceneGetEntities(manager->scene.get(), manager->layer))
		{
			IgnisColorRGBA color = IGNIS_WHITE;
			ignisBlendColorRGBA(&color, 0.4f);

			auto tex = entity->GetComponent<TextureComponent>();

			if (tex != nullptr)
			{
				glm::vec2 position = entity->GetPosition();

				glm::vec2 min = position - glm::vec2(tex->GetWidth() / 2.0f, 0.0f);
				glm::vec2 max = min + tex->GetDimension();

				Primitives2DRenderRect(min.x, min.y, max.x - min.x, max.y - min.y, color);
			}
		}

		if (manager->hover)
		{
			auto tex = manager->hover->GetComponent<TextureComponent>();

			if (tex != nullptr)
			{
				glm::vec2 position = manager->hover->GetPosition();

				glm::vec2 min = position - glm::vec2(tex->GetWidth() / 2.0f, 0.0f);
				glm::vec2 max = min + tex->GetDimension();

				Primitives2DRenderRect(min.x, min.y, max.x - min.x, max.y - min.y, IGNIS_WHITE);
				Primitives2DRenderCircle(position.x, position.y, 2.0f, IGNIS_WHITE);
			}
		}

		Primitives2DFlush();
	}
}

void SceneManagerOnRenderDebug(SceneManager* manager)
{
	if (!manager->editmode)
		SceneOnRenderDebug(manager->scene.get());
}

void SceneManagerOnImGui(SceneManager* manager)
{
	if (manager->editmode)
	{
		ImGui::Begin("Editor");

		ImGui::Text("Hovered Entity: %s", manager->hover == nullptr ? "null" : manager->hover->GetName().c_str());

		ImGui::Checkbox("Show grid", &manager->showgrid);

		ImGui::Separator();

		for (size_t i : SceneGetUsedLayers(manager->scene.get()))
			ImGui::RadioButton(obelisk::format("Layer: %zu", i).c_str(), &manager->layer, (int)i);

		ImGui::End();
	}
}
