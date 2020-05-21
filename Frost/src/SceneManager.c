#include "SceneManager.h"

#include "json/tb_json.h"
#include "json/tb_jwrite.h"

#include "Application/Application.h"
#include "Application/Debugger.h"

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

	clib_strmap_init(&manager->scenes, 0);
	clib_strmap_init(&manager->templates, 0);

	return 1;
}

void SceneManagerDelete(SceneManager* manager)
{
	if (manager->scene_name[0] != '\0')
		SceneQuit(manager->scene);

	free(manager->scene);
	clib_strmap_destroy(&manager->scenes);
	clib_strmap_destroy(&manager->templates);
}

void SceneManagerLoadRegister(SceneManager* manager, const char* json_path)
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

	tb_json_element templates;
	tb_json_read(json, &templates, "{'templates'");

	for (int i = 0; i < templates.elements; i++)
	{
		char name[APPLICATION_STR_LEN];
		tb_json_string((char*)templates.value, "{*", name, APPLICATION_STR_LEN, &i);

		tb_json_element templ;
		tb_json_read_format((char*)templates.value, &templ, "{'%s'", name);

		char path[APPLICATION_PATH_LEN];
		strncpy(path, (char*)templ.value, templ.bytelen);

		path[templ.bytelen] = '\0';

		SceneManagerRegisterTemplate(manager, name, path);
	}

	free(json);
}

void SceneManagerRegisterScene(SceneManager* manager, const char* name, const char* path)
{
	if (!clib_strmap_put(&manager->scenes, name, path))
	{
		DEBUG_ERROR("[SceneManager] Failed to add scene: %s (%s)", name, path);
	}
}

void SceneManagerRegisterTemplate(SceneManager* manager, const char* name, const char* path)
{
	if (!clib_strmap_put(&manager->templates, name, path))
	{
		DEBUG_ERROR("[SceneManager] Failed to add template: %s (%s)", name, path);
	}
}

void SceneManagerChangeScene(SceneManager* manager, const char* name)
{
	if (strcmp(manager->scene_name, name) != 0)
	{
		/* Check if scene is in the register */
		char* templ = clib_strmap_get(&manager->scenes, name);
		if (!templ)
		{
			DEBUG_ERROR("Couldn't find scene: %s\n", name);
			return;
		}
		
		char* json = ignisReadFile(templ, NULL);

		if (!json)
		{
			DEBUG_ERROR("Couldn't read scene template: %s\n", templ);
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

int SceneManagerSaveScene(SceneManager* manager, Scene* scene, const char* path)
{
	tb_jwrite_control jwc;
	tb_jwrite_open(&jwc, path, TB_JWRITE_OBJECT, TB_JWRITE_NEWLINE);

	/* size */
	tb_jwrite_array(&jwc, "size");
	tb_jwrite_set_style(&jwc, TB_JWRITE_INLINE);
	tb_jwrite_array_float(&jwc, scene->width);
	tb_jwrite_array_float(&jwc, scene->height);
	tb_jwrite_end(&jwc);

	/* background */
	tb_jwrite_set_style(&jwc, TB_JWRITE_NEWLINE);
	tb_jwrite_array(&jwc, "background");

	for (int i = 0; i < scene->background.size; ++i)
	{
		BackgroundLayer* bg = &scene->background.layers[i];

		tb_jwrite_array_array(&jwc);

		tb_jwrite_set_style(&jwc, TB_JWRITE_INLINE);
		tb_jwrite_array_string(&jwc, ResourceManagerGetTexture2DName(manager->resources, bg->texture));

		tb_jwrite_array_float(&jwc, bg->startpos);
		tb_jwrite_array_float(&jwc, bg->pos_y);
		tb_jwrite_array_float(&jwc, bg->width);
		tb_jwrite_array_float(&jwc, bg->height);
		tb_jwrite_array_float(&jwc, bg->parallax);
		tb_jwrite_end(&jwc);

		tb_jwrite_set_style(&jwc, TB_JWRITE_NEWLINE);
	}

	tb_jwrite_end(&jwc);

	/* templates */
	tb_jwrite_array(&jwc, "templates");

	for (int l = 0; l < scene->max_layer; ++l)
	{
		for (int i = 0; i < scene->layers[l].size; ++i)
		{
			EcsEntity* e = layer_vector_get(&scene->layers[l], i);

			tb_jwrite_array_array(&jwc);

			tb_jwrite_set_style(&jwc, TB_JWRITE_INLINE);

			/* template-src */
			tb_jwrite_array_string(&jwc, e->template);

			/* pos */
			vec2 pos = EcsEntityGetPosition(e);

			tb_jwrite_array_array(&jwc);
			tb_jwrite_array_float(&jwc, pos.x);
			tb_jwrite_array_float(&jwc, pos.y);
			tb_jwrite_end(&jwc);

			/* layer */
			tb_jwrite_array_int(&jwc, l);

			tb_jwrite_end(&jwc);

			tb_jwrite_set_style(&jwc, TB_JWRITE_NEWLINE);
		}
	}

	tb_jwrite_end(&jwc);

	tb_jwrite_error err = tb_jwrite_close(&jwc);

	if (err != TB_JWRITE_OK)
	{
		printf("Error: %s at function call %d\n", tb_jwrite_error_string(err), tb_jwrite_error_pos(&jwc));
		return 0;
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

char* SceneManagerGetTemplate(SceneManager* manager, const char* name)
{
	char* templ = clib_strmap_get(&manager->templates, name);
	if (!templ)
	{
		DEBUG_ERROR("Couldn't find template: %s\n", name);
		return NULL;
	}

	return templ;
}
