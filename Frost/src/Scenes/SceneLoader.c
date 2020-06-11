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

	if (!SceneLoad(scene, camera, width, height))
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
			tb_json_element entity_template;
			value = tb_json_array_step(value, &entity_template);

			char name[APPLICATION_STR_LEN];
			tb_json_string((char*)entity_template.value, "[0", name, APPLICATION_STR_LEN, NULL);
			char temp[APPLICATION_STR_LEN];
			tb_json_string((char*)entity_template.value, "[1", temp, APPLICATION_STR_LEN, NULL);

			vec2 pos;
			pos.x = tb_json_float((char*)entity_template.value, "[2[0", NULL);
			pos.y = tb_json_float((char*)entity_template.value, "[2[1", NULL);

			int z_index = tb_json_int((char*)entity_template.value, "[3", NULL);

			char* path = clib_strmap_get(templates, temp);
			if (!path)
			{
				DEBUG_WARN("[Scenes] Couldn't find template for %s\n", temp);
				continue;
			}

			if (SceneLoaderLoadTemplate(name, path, &scene->components, resources))
			{
				EcsEntity entity;
				EcsEntityLoad(&entity, name, temp);

				SceneAddEntityPos(scene, &entity, z_index, pos);
			}
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

	for (int i = 0; i < scene->entities.used; ++i)
	{
		EcsEntity* e = (EcsEntity*)clib_array_get(&scene->entities, i);

		tb_jwrite_array_array(&jwc);

		tb_jwrite_set_style(&jwc, TB_JWRITE_INLINE);

		/* name */
		tb_jwrite_array_string(&jwc, e->name);

		/* template-src */
		tb_jwrite_array_string(&jwc, e->template);

		/* pos */
		vec2 pos = ComponentTableGetEntityPosition(&scene->components, e->name);

		tb_jwrite_array_array(&jwc);
		tb_jwrite_array_float(&jwc, pos.x);
		tb_jwrite_array_float(&jwc, pos.y);
		tb_jwrite_end(&jwc);

		/* z_index */
		tb_jwrite_array_int(&jwc, e->z_index);

		tb_jwrite_end(&jwc);

		tb_jwrite_set_style(&jwc, TB_JWRITE_NEWLINE);
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

int SceneLoaderLoadTemplate(const char* entity, const char* path, ComponentTable* components, ResourceManager* resources)
{
	char* json = ignisReadFile(path, NULL);

	if (!json)
	{
		DEBUG_WARN("[Scenes] Couldn't read template (%s)", path);
		return 0;
	}

	tb_json_element element;
	tb_json_read(json, &element, "{'position'");
	if (element.error == TB_JSON_OK)
	{
		EcsPositionComponent comp;
		comp.x = tb_json_float((char*)element.value, "[0", NULL);
		comp.y = tb_json_float((char*)element.value, "[1", NULL);

		ComponentTableAddComponent(components, entity, COMPONENT_POSITION, &comp);
	}

	tb_json_read(json, &element, "{'physics'");
	if (element.error == TB_JSON_OK)
	{
		tb_json_element body;
		tb_json_read((char*)element.value, &body, "{'body'");

		if (body.error == TB_JSON_OK)
		{
			EcsPhysicsComponent comp;
			BodyType type = (BodyType)tb_json_int((char*)body.value, "{'type'", NULL);

			float x = tb_json_float((char*)body.value, "{'position'[0", NULL);
			float y = tb_json_float((char*)body.value, "{'position'[1", NULL);

			float w = tb_json_float((char*)body.value, "{'halfDimension'[0", NULL);
			float h = tb_json_float((char*)body.value, "{'halfDimension'[1", NULL);

			comp.body_x = tb_json_float((char*)element.value, "{'offset'[0", NULL);
			comp.body_y = tb_json_float((char*)element.value, "{'offset'[1", NULL);

			BodyLoad(&comp.body, x, y, w, h, type);

			ComponentTableAddComponent(components, entity, COMPONENT_PHYSICS, &comp);
		}
	}

	tb_json_read(json, &element, "{'texture'");
	if (element.error == TB_JSON_OK)
	{
		char tex_name[APPLICATION_STR_LEN];
		tb_json_string((char*)element.value, "{'name'", tex_name, APPLICATION_STR_LEN, NULL);

		EcsTextureComponent comp;
		comp.render_flip = RENDER_FLIP_NONE;
		comp.width = tb_json_float((char*)element.value, "{'dimension'[0", NULL);
		comp.height = tb_json_float((char*)element.value, "{'dimension'[1", NULL);

		comp.frame = tb_json_int((char*)element.value, "{'frame'", NULL);

		comp.texture = ResourceManagerGetTexture2D(resources, tex_name);

		if (comp.texture)
			ComponentTableAddComponent(components, entity, COMPONENT_TEXTURE, &comp);
	}

	tb_json_read(json, &element, "{'animation'");
	if (element.error == TB_JSON_OK)
	{
		EcsAnimationComponent comp;
		AnimationComponentInit(&comp);

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

			AnimationComponentAddAnimation(&comp, anim_name, animation);
		}
		ComponentTableAddComponent(components, entity, COMPONENT_ANIMATION, &comp);
	}

	tb_json_read(json, &element, "{'movement'");
	if (element.error == TB_JSON_OK)
	{
		EcsMovementComponent comp;

		comp.direction = DIRECTION_LEFT;
		comp.speed = tb_json_float((char*)element.value, "{'speed'", NULL);
		comp.jump_power = tb_json_float((char*)element.value, "{'jumppower'", NULL);

		ComponentTableAddComponent(components, entity, COMPONENT_MOVEMENT, &comp);
	}

	tb_json_read(json, &element, "{'camera'");
	if (element.error == TB_JSON_OK)
	{
		EcsCameraComponent comp;
		comp.camera = NULL;
		comp.smooth = tb_json_float((char*)element.value, "{'smooth'", NULL);
		comp.scene_w = -1.0f;
		comp.scene_h = -1.0f;

		ComponentTableAddComponent(components, entity, COMPONENT_CAMERA, &comp);
	}

	tb_json_read(json, &element, "{'interaction'");
	if (element.error == TB_JSON_OK)
	{
		EcsInteractionComponent comp;
		comp.radius = tb_json_float((char*)element.value, "{'radius'", NULL);
		comp.type = (InteractionType)tb_json_int((char*)element.value, "{'type'", NULL);

		ComponentTableAddComponent(components, entity, COMPONENT_INTERACTION, &comp);
	}

	tb_json_read(json, &element, "{'interactor'");
	if (element.error == TB_JSON_OK)
	{
		EcsInteractorComponent comp;
		comp.type = (InteractionType)tb_json_int((char*)element.value, "{'type'", NULL);

		ComponentTableAddComponent(components, entity, COMPONENT_INTERACTOR, &comp);
	}

	free(json);

	return 1;
}