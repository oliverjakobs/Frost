#include "SceneManager.hpp"

#include "json/TemplateLoader.hpp"

#include "Application.hpp"

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
		manager->scene->OnExtit();
}

void SceneManagerRegisterScenes(SceneManager* manager, const std::string& path)
{
	TemplateLoadSceneRegister(manager, path.c_str());
}

void SceneManagerRegisterScene(SceneManager* manager, const std::string& name, const std::string& path)
{
	if (!(name.empty() || path.empty()))
		manager->scenes.insert({ name, path });
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
				manager->scene->OnExtit();

			manager->scene = newScene;
			manager->sceneName = name;

			// Enter new scene
			manager->scene->OnEntry();
		}
		else
		{
			OBELISK_WARN("Failed to load scene: %s", name.c_str());
		}
	}
}

std::shared_ptr<Scene> SceneManagerLoadScene(SceneManager* manager, const std::string& name)
{
	auto path = manager->scenes.find(name);

	if (path == manager->scenes.end())
	{
		OBELISK_WARN("Couldn't find scene: ", name.c_str());
		return nullptr;
	}

	return TemplateLoadScene(path->second.c_str(), manager->camera, manager->resources);
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
			manager->scene->RemoveEntity("tree", 0);
			break;
		}
	}

	manager->scene->OnEvent(e);
}

void SceneManagerOnUpdate(SceneManager* manager, float deltaTime)
{
	if (!manager->editmode)
	{
		manager->scene->OnUpdate(deltaTime);
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

		manager->hover = manager->scene->GetEntityAt(CameraGetMousePos(manager->camera, InputMousePosition()), manager->layer).get();
	}
}

void SceneManagerOnRender(SceneManager* manager)
{
	manager->scene->OnRender();

	if (manager->editmode)
	{
		// render grid
		Primitives2DStart(CameraGetViewProjectionPtr(manager->camera));

		if (manager->showgrid)
		{
			IgnisColorRGBA color = IGNIS_WHITE;
			ignisBlendColorRGBA(&color, 0.2f);

			for (float x = -manager->padding; x <= manager->scene->GetWidth() + manager->padding; x += manager->gridsize)
				Primitives2DRenderLine(x, -manager->padding, x, manager->scene->GetHeight() + manager->padding, color);

			for (float y = -manager->padding; y <= manager->scene->GetHeight() + manager->padding; y += manager->gridsize)
				Primitives2DRenderLine(-manager->padding, y, manager->scene->GetWidth() + manager->padding, y, color);
		}

		for (auto& entity : manager->scene->GetEntities(manager->layer))
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
		manager->scene->OnRenderDebug();
}

void SceneManagerOnImGui(SceneManager* manager)
{
	if (manager->editmode)
	{
		ImGui::Begin("Editor");

		ImGui::Text("Hovered Entity: %s", manager->hover == nullptr ? "null" : manager->hover->GetName().c_str());

		ImGui::Checkbox("Show grid", &manager->showgrid);

		ImGui::Separator();

		for (size_t i : manager->scene->GetLayers())
			ImGui::RadioButton(obelisk::format("Layer: %zu", i).c_str(), &manager->layer, (int)i);

		ImGui::End();
	}
}
