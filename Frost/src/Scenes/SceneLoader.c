#include "SceneLoader.h"

#include "json/tb_json.h"
#include "json/tb_jwrite.h"

#include "Application/Debugger.h"

int SceneLoaderLoadRegister(SceneManager* manager, const char* path)
{
	char* json = ignisReadFile(path, NULL);

	if (!json)
	{
		DEBUG_ERROR("[Scenes] Failed to read register (%s)\n", path);
		return 0;
	}

	tb_json_element scenes;
	tb_json_read(json, &scenes, "{'scenes'");

	for (int i = 0; i < scenes.elements; i++)
	{
		char name[APPLICATION_STR_LEN];
		tb_json_string((char*)scenes.value, "{*", name, APPLICATION_STR_LEN, &i);

		tb_json_element scene;
		tb_json_read_format((char*)scenes.value, &scene, "{'%s'", name);

		char scene_path[APPLICATION_PATH_LEN];
		strncpy(scene_path, (char*)scene.value, scene.bytelen);

		scene_path[scene.bytelen] = '\0';

		if (!clib_strmap_insert(&manager->scenes, name, scene_path))
		{
			DEBUG_WARN("[Scenes] Failed to add scene: %s (%s)", name, scene_path);
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

		char templ_path[APPLICATION_PATH_LEN];
		strncpy(templ_path, (char*)templ.value, templ.bytelen);

		templ_path[templ.bytelen] = '\0';

		if (!clib_strmap_insert(&manager->templates, name, templ_path))
		{
			DEBUG_WARN("[Scenes] Failed to add template: %s (%s)", name, templ_path);
		}
	}

	free(json);

	return 1;
}

int SceneLoaderLoadScene(Scene* scene, const char* path, Camera* camera, ResourceManager* resources, clib_strmap* templates)
{
	char* json = ignisReadFile(path, NULL);

	if (!json)
	{
		DEBUG_ERROR("[Scenes] Couldn't read scene template: %s\n", path);
		return 0;
	}

	float width = tb_json_float((char*)json, "{'size'[0", NULL);
	float height = tb_json_float((char*)json, "{'size'[1", NULL);

	if (!SceneLoad(scene, camera, width, height, SCENE_MANAGER_LAYER_COUNT))
	{
		DEBUG_ERROR("[Scenes] Failed to load scene: %s", json);
		free(json);
		return 0;
	}

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

			BackgroundPushLayer(&scene->background, ResourceManagerGetTexture2D(resources, name), x, y, w, h, parallax);
		}
	}

	tb_json_element templs;
	tb_json_read((char*)json, &templs, "{'templates'");

	if (templs.error == TB_JSON_OK && templs.data_type == TB_JSON_ARRAY)
	{
		char* value = (char*)templs.value;

		for (int i = 0; i < templs.elements; i++)
		{
			tb_json_element entity;
			value = tb_json_array_step(value, &entity);

			char name[APPLICATION_STR_LEN];
			tb_json_string((char*)entity.value, "[0", name, APPLICATION_STR_LEN, NULL);

			vec2 pos;
			pos.x = tb_json_float((char*)entity.value, "[1[0", NULL);
			pos.y = tb_json_float((char*)entity.value, "[1[1", NULL);

			int layer = tb_json_int((char*)entity.value, "[2", NULL);

			SceneAddEntityPos(scene, SceneLoaderLoadTemplate(name, templates, resources), layer, pos);
		}
	}

	free(json);
	return 1;
}

int SceneLoaderSaveScene(Scene* scene, const char* path, ResourceManager* resources)
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
		tb_jwrite_array_string(&jwc, ResourceManagerGetTexture2DName(resources, bg->texture));

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
			EcsEntity* e = layer_dynamic_array_get(&scene->layers[l], i);

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
		DEBUG_ERROR("[Scenes] Failed to remove old save file (%s)\n", path);
		return 0;
	}

	if (rename(temp_path, path) != 0)
	{
		DEBUG_ERROR("[Scenes] Failed to rename temp save file (%s)\n", temp_path);
		return 0;
	}

	return 1;
}

EcsEntity* SceneLoaderLoadTemplate(const char* templ, clib_strmap* templates, ResourceManager* resources)
{
	char* path = clib_strmap_get(templates, templ);

	if (!path)
	{
		DEBUG_WARN("[Scenes] Couldn't find template for %s\n", templ);
		return NULL;
	}

	char* json = ignisReadFile(path, NULL);

	if (!json)
	{
		DEBUG_WARN("[Scenes] Couldn't read template (%s)", path);
		return NULL;
	}

	char name[APPLICATION_STR_LEN];
	tb_json_string(json, "{'name'", name, APPLICATION_STR_LEN, NULL);

	EcsEntity* entity = (EcsEntity*)malloc(sizeof(EcsEntity));
	EcsEntityLoad(entity, name, templ);

	tb_json_element element;
	tb_json_read(json, &element, "{'position'");
	if (element.error == TB_JSON_OK)
	{
		float x = tb_json_float((char*)element.value, "[0", NULL);
		float y = tb_json_float((char*)element.value, "[1", NULL);

		EcsEntityAddPosition(entity, x, y);
	}

	tb_json_read(json, &element, "{'physics'");
	if (element.error == TB_JSON_OK)
	{
		tb_json_element body;
		tb_json_read((char*)element.value, &body, "{'body'");

		if (body.error == TB_JSON_OK)
		{
			BodyType type = (BodyType)tb_json_int((char*)body.value, "{'type'", NULL);

			float x = tb_json_float((char*)body.value, "{'position'[0", NULL);
			float y = tb_json_float((char*)body.value, "{'position'[1", NULL);

			float w = tb_json_float((char*)body.value, "{'halfDimension'[0", NULL);
			float h = tb_json_float((char*)body.value, "{'halfDimension'[1", NULL);

			float offset_x = tb_json_float((char*)element.value, "{'offset'[0", NULL);
			float offset_y = tb_json_float((char*)element.value, "{'offset'[1", NULL);

			Body* body = (Body*)malloc(sizeof(Body));
			BodyLoad(body, x, y, w, h, type);

			EcsEntityAddPhysics(entity, body, offset_x, offset_y);
		}
	}

	tb_json_read(json, &element, "{'texture'");
	if (element.error == TB_JSON_OK)
	{
		char tex_name[APPLICATION_STR_LEN];
		tb_json_string((char*)element.value, "{'name'", tex_name, APPLICATION_STR_LEN, NULL);

		float width = tb_json_float((char*)element.value, "{'dimension'[0", NULL);
		float height = tb_json_float((char*)element.value, "{'dimension'[1", NULL);

		int frame = tb_json_int((char*)element.value, "{'frame'", NULL);

		IgnisTexture2D* texture = ResourceManagerGetTexture2D(resources, tex_name);

		if (texture)
			EcsEntityAddTexture(entity, texture, width, height, frame);
	}

	tb_json_read(json, &element, "{'animation'");
	if (element.error == TB_JSON_OK)
	{
		Animator* animator = (Animator*)malloc(sizeof(Animator));
		AnimatorInit(animator);

		for (int i = 0; i < element.elements; i++)
		{
			char anim_name[APPLICATION_STR_LEN];
			tb_json_string((char*)element.value, "{*", anim_name, APPLICATION_STR_LEN, &i);

			tb_json_element anim;
			tb_json_read_format((char*)element.value, &anim, "{'%s'", anim_name);

			unsigned int start = tb_json_int((char*)anim.value, "{'start'", NULL);
			unsigned int length = tb_json_int((char*)anim.value, "{'length'", NULL);
			float delay = tb_json_float((char*)anim.value, "{'delay'", NULL);

			tb_json_element transition_array;
			tb_json_read((char*)anim.value, &transition_array, "{'transitions'");

			Animation* animation = (Animation*)malloc(sizeof(Animation));
			AnimationLoad(animation, start, length, delay, transition_array.elements);

			char* value = (char*)transition_array.value;
			tb_json_element transition_element;
			for (int i = 0; i < transition_array.elements; i++)
			{
				value = tb_json_array_step(value, &transition_element);

				if (transition_element.data_type == TB_JSON_ARRAY)
				{
					char condition[APPLICATION_STR_LEN];
					tb_json_string((char*)transition_element.value, "[0", condition, APPLICATION_STR_LEN, NULL);
					char next[APPLICATION_STR_LEN];
					tb_json_string((char*)transition_element.value, "[1", next, APPLICATION_STR_LEN, NULL);

					AnimationAddTransition(animation, condition, next);
				}
			}

			AnimatorAddAnimation(animator, anim_name, animation);
		}
		/* AnimatorDebugPrint(animator); */
		EcsEntityAddAnimation(entity, animator);
	}

	tb_json_read(json, &element, "{'movement'");
	if (element.error == TB_JSON_OK)
	{
		float ms = tb_json_float((char*)element.value, "{'speed'", NULL);
		float jp = tb_json_float((char*)element.value, "{'jumppower'", NULL);

		EcsEntityAddMovement(entity, ms, jp);
	}

	tb_json_read(json, &element, "{'camera'");
	if (element.error == TB_JSON_OK)
	{
		float smooth = tb_json_float((char*)element.value, "{'smooth'", NULL);

		EcsEntityAddCamera(entity, smooth);
	}

	tb_json_read(json, &element, "{'interaction'");
	if (element.error == TB_JSON_OK)
	{
		float radius = tb_json_float((char*)element.value, "{'radius'", NULL);

		EcsEntityAddInteraction(entity, radius);
	}

	free(json);

	return entity;
}