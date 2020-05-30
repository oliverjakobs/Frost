#include "SceneLoader.h"

#include "json/tb_json.h"
#include "json/tb_jwrite.h"

#include "ECS/TemplateLoader.h"
#include "Application/Debugger.h"

void SceneLoaderLoadRegister(SceneManager* manager, const char* json_path)
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

		if (!clib_strmap_insert(&manager->scenes, name, path))
		{
			DEBUG_ERROR("[SceneManager] Failed to add scene: %s (%s)", name, path);
		}
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

		if (!clib_strmap_insert(&manager->templates, name, path))
		{
			DEBUG_ERROR("[SceneManager] Failed to add template: %s (%s)", name, path);
		}
	}

	free(json);
}

int SceneLoaderLoadScene(SceneManager* manager, Scene* scene, const char* json)
{
	float width = tb_json_float((char*)json, "{'size'[0", NULL);
	float height = tb_json_float((char*)json, "{'size'[1", NULL);

	SceneLoad(scene, manager->camera, width, height, SCENE_MANAGER_LAYER_COUNT);

	tb_json_element background;
	tb_json_read((char*)json, &background, "{'background'");

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
	tb_json_read((char*)json, &templates, "{'templates'");

	if (templates.error == TB_JSON_OK && templates.data_type == TB_JSON_ARRAY)
	{
		char* value = (char*)templates.value;

		for (int i = 0; i < templates.elements; i++)
		{
			tb_json_element entity;
			value = tb_json_array_step(value, &entity);

			char name[APPLICATION_STR_LEN];
			tb_json_string((char*)entity.value, "[0", name, APPLICATION_STR_LEN, NULL);

			vec2 pos;
			pos.x = tb_json_float((char*)entity.value, "[1[0", NULL);
			pos.y = tb_json_float((char*)entity.value, "[1[1", NULL);

			int layer = tb_json_int((char*)entity.value, "[2", NULL);

			SceneAddEntityPos(scene, EcsEntityLoadTemplate(manager, name), layer, pos);
		}
	}

	return 1;
}

int SceneLoaderSaveScene(SceneManager* manager, Scene* scene, const char* path)
{
	char* temp_ext = ".temp";
	char* temp_path = (char*)malloc(strlen(path) + strlen(temp_ext));

	strcpy(temp_path, path);
	strcat(temp_path, temp_ext);

	tb_jwrite_control jwc;
	tb_jwrite_open(&jwc, temp_path, TB_JWRITE_OBJECT, TB_JWRITE_NEWLINE);
	tb_jwrite_set_float_prec(&jwc, 2);

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
		DEBUG_ERROR("[JWRITE] Error: %s at function call %d\n", tb_jwrite_error_string(err), tb_jwrite_error_pos(&jwc));
		return 0;
	}

	if (remove(path) != 0)
	{
		DEBUG_ERROR("[SceneManager] Failed to remove old save file (%s)\n", path);
		return 0;
	}

	if (rename(temp_path, path) != 0)
	{
		DEBUG_ERROR("[SceneManager] Failed to rename temp save file (%s)\n", temp_path);
		return 0;
	}

	return 1;
}

char* SceneLoaderGetTemplate(SceneManager* manager, const char* name)
{
	char* templ = clib_strmap_get(&manager->templates, name);
	if (!templ)
	{
		DEBUG_ERROR("[SceneManager] Couldn't find template: %s\n", name);
		return NULL;
	}

	return templ;
}