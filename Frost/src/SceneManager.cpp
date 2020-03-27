#include "SceneManager.hpp"

#include "json/TemplateLoader.hpp"
#include "json/tb_json.h"

#include "Application.hpp"

#define SCENE_MANAGER_NAMELEN	32
#define SCENE_MANAGER_PATHLEN	64

#define SCENE_MANAGER_LAYER_COUNT	4

typedef struct
{
	char key[SCENE_MANAGER_NAMELEN];
	char value[SCENE_MANAGER_PATHLEN];
} _scenekvp;

_scenekvp* _scene_kvp(const char* key, const char* value)
{
	if (strlen(key) > SCENE_MANAGER_NAMELEN)
		return NULL;

	if (strlen(value) > SCENE_MANAGER_PATHLEN)
		return NULL;

	_scenekvp* kvp = (_scenekvp*)malloc(sizeof(_scenekvp));

	if (kvp)
	{
		strcpy(kvp->key, key);
		strcpy(kvp->value, value);
	}

	return kvp;
}

CLIB_HASHMAP_DEFINE_FUNCS(scene, char, _scenekvp)

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

	manager->scene = (Scene*)malloc(sizeof(Scene));
	if (!manager->scene)
		return 0;

	manager->sceneName = "";

	clib_hashmap_init(&manager->scenes, clib_hashmap_hash_string, clib_hashmap_compare_string, 0);

	return 1;
}

void SceneManagerDelete(SceneManager* manager)
{
	if (!manager->sceneName.empty())
		SceneQuit(manager->scene);

	free(manager->scene);
	clib_hashmap_destroy(&manager->scenes);
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

void SceneManagerRegisterScene(SceneManager* manager, const char* name, const char* path)
{
	_scenekvp* kvp = _scene_kvp(name, path);

	if (kvp && scene_hashmap_put(&manager->scenes, kvp->key, kvp) != kvp)
	{
		free(kvp);
	}
}

void SceneManagerChangeScene(SceneManager* manager, const char* name)
{
	if (!obelisk::StringCompare(manager->sceneName, name))
	{
		/* Check if scene is in the register */
		_scenekvp* kvp = scene_hashmap_get(&manager->scenes, name);
		if (!kvp)
		{
			OBELISK_WARN("Couldn't find scene: ", name);
			return;
		}
		
		char* json = ignisReadFile(kvp->value, NULL);

		if (!json)
		{
			OBELISK_WARN("Couldn't read scene template: ", kvp->value);
			return;
		}

		// Exit old Scene
		if (!manager->sceneName.empty())
			SceneQuit(manager->scene);

		// Enter new scene
		SceneManagerLoadScene(manager, manager->scene, json);
		manager->sceneName = name;

		free(json);
	}
}

int SceneManagerLoadScene(SceneManager* manager, Scene* scene, const char* templ)
{
	char* json = (char*)templ;

	float width = tb_json_float(json, "{'size'[0", NULL);
	float height = tb_json_float(json, "{'size'[1", NULL);

	SceneLoad(scene, manager->camera, width, height, SCENE_MANAGER_LAYER_COUNT);

	tb_json_element background;
	tb_json_read(json, &background, "{'background'");

	if (background.error == TB_JSON_OK && background.data_type == TB_JSON_ARRAY)
	{
		char* value = (char*)background.value;

		BackgroundInit(&scene->background, background.elements);
		for (int i = 0; i < background.elements; i++)
		{
			tb_json_element entity;
			value = tb_json_array_step(value, &entity);

			char name[SCENE_MANAGER_NAMELEN];
			tb_json_string((char*)entity.value, "[0", name, SCENE_MANAGER_NAMELEN, NULL);

			float x = tb_json_float((char*)entity.value, "[1", NULL);
			float y = tb_json_float((char*)entity.value, "[2", NULL);

			float w = tb_json_float((char*)entity.value, "[3", NULL);
			float h = tb_json_float((char*)entity.value, "[4", NULL);

			float parallax = tb_json_float((char*)entity.value, "[5", NULL);

			BackgroundPushLayer(&scene->background, ResourceManagerGetTexture(manager->resources, name), x, y, w, h, parallax);
		}
	}


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

			SceneAddEntity(scene, TemplateLoadEntity(path, manager->resources), layer, glm::vec2(x, y));
		}
	}

	return 1;
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
			SceneClearEntities(manager->scene);
			break;
		}
	}

	SceneOnEvent(manager->scene, e);
}

void SceneManagerOnUpdate(SceneManager* manager, float deltaTime)
{
	if (!manager->editmode)
	{
		SceneOnUpdate(manager->scene, deltaTime);
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

		manager->hover = SceneGetEntityAt(manager->scene, CameraGetMousePos(manager->camera, InputMousePosition()), manager->layer);
	}
}

void SceneManagerOnRender(SceneManager* manager)
{
	SceneOnRender(manager->scene);

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

		clib_vector* layer = SceneGetLayer(manager->scene, manager->layer);

		if (layer)
		{
			for (size_t i = 0; i < layer->size; i++)
			{
				IgnisColorRGBA color = IGNIS_WHITE;
				ignisBlendColorRGBA(&color, 0.4f);

				auto tex = EntityGetComponent<TextureComponent>(layer_vector_get(layer, i));

				if (tex != nullptr)
				{
					glm::vec2 position = EntityGetPosition(layer_vector_get(layer, i));

					glm::vec2 min = position - glm::vec2(tex->GetWidth() / 2.0f, 0.0f);
					glm::vec2 max = min + tex->GetDimension();

					Primitives2DRenderRect(min.x, min.y, max.x - min.x, max.y - min.y, color);
				}
			}

		}

		if (manager->hover)
		{
			auto tex = EntityGetComponent<TextureComponent>(manager->hover);

			if (tex != nullptr)
			{
				glm::vec2 position = EntityGetPosition(manager->hover);

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
		SceneOnRenderDebug(manager->scene);
}

void SceneManagerOnImGui(SceneManager* manager)
{
	if (manager->editmode)
	{
		ImGui::Begin("Editor");

		ImGui::Text("Hovered Entity: %s", manager->hover == nullptr ? "null" : manager->hover->name.c_str());

		ImGui::Checkbox("Show grid", &manager->showgrid);

		ImGui::Separator();

		for (size_t i : SceneGetUsedLayers(manager->scene))
			ImGui::RadioButton(obelisk::format("Layer: %zu", i).c_str(), &manager->layer, (int)i);

		ImGui::End();
	}
}
