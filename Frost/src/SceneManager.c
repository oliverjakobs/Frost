#include "SceneManager.h"

#include "json/tb_json.h"

#include "Application/Application.h"
#include "Application/Debugger.h"

typedef struct
{
	char key[APPLICATION_STR_LEN];
	char value[APPLICATION_PATH_LEN];
} _scenekvp;

CLIB_HASHMAP_DEFINE_FUNCS(scene, char, _scenekvp)

int SceneManagerInit(SceneManager* manager, ResourceManager* resources, Camera* camera, float gridsize, uint16_t padding)
{
	manager->resources = resources;
	manager->camera = camera;

	manager->editmode = 1;
	SceneEditorInit(&manager->editor, 400.0f, gridsize, gridsize * padding);

	manager->scene = (Scene*)malloc(sizeof(Scene));
	if (!manager->scene)
	{
		DEBUG_ERROR("[SceneManager] Failed to allocate memory for scene name\n");
		return 0;
	}
	memset(manager->scene_name, '\0', APPLICATION_STR_LEN);

	clib_hashmap_init(&manager->scenes, clib_hashmap_hash_string, clib_hashmap_compare_string, 0);

	return 1;
}

void SceneManagerDelete(SceneManager* manager)
{
	if (manager->scene_name[0] != '\0')
		SceneQuit(manager->scene);

	free(manager->scene);
	clib_hashmap_destroy(&manager->scenes);
}

void SceneManagerRegisterScenes(SceneManager* manager, const char* json_path)
{
	char* json = ignisReadFile(json_path, NULL);

	if (!json)
	{
		DEBUG_ERROR("[SceneManager] Failed to read register (%s)\n", json_path);
		return;
	}

	tb_json_element scenes;
	tb_json_read(json, &scenes, "{'scenes'");

	for (int i = 0; i < scenes.elements; i++)
	{
		char name[APPLICATION_STR_LEN];
		tb_json_string((char*)scenes.value, "{*", name, APPLICATION_STR_LEN, &i);

		tb_json_element scene;
		tb_json_read_format((char*)scenes.value, &scene, "{'%s'", name);

		char path[APPLICATION_PATH_LEN];
		strncpy(path, (char*)scene.value, scene.bytelen);

		path[scene.bytelen] = '\0';

		SceneManagerRegisterScene(manager, name, path);
	}

	free(json);
}

void SceneManagerRegisterScene(SceneManager* manager, const char* name, const char* path)
{
	if (strlen(name) > APPLICATION_STR_LEN)
	{
		DEBUG_ERROR("[SceneManager] Scene name (%s) too long. Max. name length is %d\n", name, APPLICATION_STR_LEN);
		return;
	}

	if (strlen(path) > APPLICATION_PATH_LEN)
	{
		DEBUG_ERROR("[SceneManager] Scene path (%s) too long. Max. name length is %d\n", path, APPLICATION_PATH_LEN);
		return;
	}

	_scenekvp* kvp = (_scenekvp*)malloc(sizeof(_scenekvp));

	if (kvp)
	{
		strcpy(kvp->key, name);
		strcpy(kvp->value, path);

		if (scene_hashmap_put(&manager->scenes, kvp->key, kvp) != kvp)
		{
			DEBUG_ERROR("[SceneManager] Failed to add scene: %s (%s)", name, path);
			free(kvp);
		}
	}
}

void SceneManagerChangeScene(SceneManager* manager, const char* name)
{
	if (strcmp(manager->scene_name, name) != 0)
	{
		/* Check if scene is in the register */
		_scenekvp* kvp = scene_hashmap_get(&manager->scenes, name);
		if (!kvp)
		{
			DEBUG_ERROR("Couldn't find scene: %s\n", name);
			return;
		}
		
		char* json = ignisReadFile(kvp->value, NULL);

		if (!json)
		{
			DEBUG_ERROR("Couldn't read scene template: %s\n", kvp->value);
			return;
		}

		// Exit old Scene
		if (manager->scene_name[0] != '\0')
			SceneQuit(manager->scene);

		// Enter new scene
		SceneManagerLoadScene(manager, manager->scene, json);
		SceneEditorReset(&manager->editor);
		strcpy(manager->scene_name, name);

		free(json);
	}
}

int SceneManagerLoadScene(SceneManager* manager, Scene* scene, const char* json)
{
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

			char name[APPLICATION_STR_LEN];
			tb_json_string((char*)entity.value, "[0", name, APPLICATION_STR_LEN, NULL);

			float x = tb_json_float((char*)entity.value, "[1", NULL);
			float y = tb_json_float((char*)entity.value, "[2", NULL);

			float w = tb_json_float((char*)entity.value, "[3", NULL);
			float h = tb_json_float((char*)entity.value, "[4", NULL);

			float parallax = tb_json_float((char*)entity.value, "[5", NULL);

			BackgroundPushLayer(&scene->background, ResourceManagerGetTexture2D(manager->resources, name), x, y, w, h, parallax);
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

			char path[APPLICATION_PATH_LEN];
			tb_json_string((char*)entity.value, "[0", path, APPLICATION_PATH_LEN, NULL);

			float x = tb_json_float((char*)entity.value, "[1[0", NULL);
			float y = tb_json_float((char*)entity.value, "[1[1", NULL);

			int layer = tb_json_int((char*)entity.value, "[2", NULL);

			SceneAddEntityPos(scene, EcsEntityLoadTemplate(path, manager->resources), layer, (vec2){ x, y });
		}
	}

	return 1;
}

void SceneManagerOnEvent(SceneManager* manager, Event e)
{
	if (e.type == EVENT_WINDOW_RESIZE)
	{
		CameraSetProjectionOrtho(manager->camera, (float)e.window.width, (float)e.window.height);
	}

	if (e.type == EVENT_KEY_PRESSED)
	{
		switch (e.key.keycode)
		{
		case KEY_F1:
			manager->editmode = !manager->editmode;
			break;
		}
	}

	if (manager->editmode)
		SceneEditorOnEvent(&manager->editor, manager->scene, e);

	SceneOnEvent(manager->scene, e);
}

void SceneManagerOnUpdate(SceneManager* manager, float deltatime)
{
	if (!manager->editmode)
	{
		SceneOnUpdate(manager->scene, deltatime);
	}
	else
	{
		SceneEditorOnUpdate(&manager->editor, manager->scene, deltatime);
	}
}

void SceneManagerOnRender(SceneManager* manager)
{
	SceneOnRender(manager->scene);

	if (manager->editmode)
	{
		SceneEditorOnRender(&manager->editor, manager->scene);
	}
}

void SceneManagerOnRenderDebug(SceneManager* manager)
{
	if (!manager->editmode)
		SceneOnRenderDebug(manager->scene);
}

void SceneManagerOnRenderGui(SceneManager* manager)
{

}
