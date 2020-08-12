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

int SceneLoaderLoadScene(SceneManager* manager, const char* path)
{
	char* json = ignisReadFile(path, NULL);

	if (!json)
	{
		DEBUG_ERROR("[Scenes] Couldn't read scene template: %s\n", path);
		return 0;
	}

	float width = tb_json_float((char*)json, "{'size'[0", NULL, 0.0f);
	float height = tb_json_float((char*)json, "{'size'[1", NULL, 0.0f);

	if (!SceneLoad(manager->scene, manager->camera, width, height))
	{
		DEBUG_ERROR("[Scenes] Failed to load scene: %s", json);
		free(json);
		return 0;
	}

	tb_json_element element;
	tb_json_read((char*)json, &element, "{'background'");

	if (element.error == TB_JSON_OK && element.data_type == TB_JSON_ARRAY)
	{
		char* value = (char*)element.value;
		Background* background = &manager->scene->background;

		BackgroundInit(background, element.elements);
		for (int i = 0; i < element.elements; i++)
		{
			tb_json_element entity;
			value = tb_json_array_step(value, &entity);

			char name[APPLICATION_STR_LEN];
			tb_json_string((char*)entity.value, "[0", name, APPLICATION_STR_LEN, NULL);

			float x = tb_json_float((char*)entity.value, "[1", NULL, 0.0f);
			float y = tb_json_float((char*)entity.value, "[2", NULL, 0.0f);

			float w = tb_json_float((char*)entity.value, "[3", NULL, 0.0f);
			float h = tb_json_float((char*)entity.value, "[4", NULL, 0.0f);

			float parallax = tb_json_float((char*)entity.value, "[5", NULL, 0.0f);

			BackgroundPushLayer(background, ResourceManagerGetTexture2D(manager->resources, name), x, y, w, h, parallax);
		}
	}

	tb_json_read((char*)json, &element, "{'templates'");

	if (element.error == TB_JSON_OK && element.data_type == TB_JSON_ARRAY)
	{
		char* value = (char*)element.value;

		for (size_t i = 0; i < element.elements; ++i)
		{
			tb_json_element entity_template;
			value = tb_json_array_step(value, &entity_template);

			char templ[APPLICATION_STR_LEN];
			tb_json_string((char*)entity_template.value, "[0", templ, APPLICATION_STR_LEN, NULL);

			vec2 pos;
			pos.x = tb_json_float((char*)entity_template.value, "[1[0", NULL, 0.0f);
			pos.y = tb_json_float((char*)entity_template.value, "[1[1", NULL, 0.0f);

			int z_index = tb_json_int((char*)entity_template.value, "[2", NULL, 0);

			/* Load Template */
			EntityID id = EntityGetNextID();
			if (!SceneLoaderLoadTemplate(manager, templ, id, pos, z_index))
			{
				DEBUG_ERROR("[Scenes] Failed to load template %s\n", templ);
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

	if (!temp_path)
	{
		DEBUG_ERROR("[Scenes] Failed to allocate memory for temp path\n");
		return 0;
	}

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

	/* TODO */
	//CLIB_HASHMAP_ITERATE_FOR(&scene->entity_templates, iter)
	//{
	//	tb_jwrite_array_array(&jwc);
	//
	//	tb_jwrite_set_style(&jwc, TB_JWRITE_INLINE);
	//
	//	/* name */
	//	tb_jwrite_array_string(&jwc, clib_strmap_iter_get_key(iter));
	//
	//	/* template-src */
	//	tb_jwrite_array_string(&jwc, clib_strmap_iter_get_value(iter));
	//
	//	/* pos */
	//	vec2 pos = EntityGetPosition(clib_strmap_iter_get_key(iter), &scene->components);
	//
	//	tb_jwrite_array_array(&jwc);
	//	tb_jwrite_array_float(&jwc, pos.x);
	//	tb_jwrite_array_float(&jwc, pos.y);
	//	tb_jwrite_end(&jwc);
	//
	//	/* z_index */
	//	tb_jwrite_array_int(&jwc, EcsGetEntityIndex(&scene->ecs, clib_strmap_iter_get_key(iter)));
	//
	//	tb_jwrite_end(&jwc);
	//
	//	tb_jwrite_set_style(&jwc, TB_JWRITE_NEWLINE);
	//}

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


int SceneLoaderLoadTemplate(SceneManager* manager, const char* templ, EntityID entity, vec2 pos, int z_index)
{
	char* path = clib_strmap_find(&manager->templates, templ);
	if (!path)
	{
		DEBUG_WARN("[Scenes] Couldn't find template for %s\n", templ);
		return 0;
	}

	char* json = ignisReadFile(path, NULL);

	if (!json)
	{
		DEBUG_WARN("[Scenes] Couldn't read template (%s)", path);
		return 0;
	}

	tb_json_element element;

	SceneAddEntityTemplate(manager->scene, entity, templ);

	Template t;
	t.entity = entity;
	t.templ = malloc(strlen(templ));
	strcpy(t.templ, templ);
	EcsAddOrderComponent(&manager->scene->ecs, COMPONENT_TEMPLATE, &t);
	/* TODO: Free template */

	/* Components */
	Transform transform;
	RigidBody body;
	Sprite sprite;

	tb_json_read(json, &element, "{'transform'");
	if (element.error == TB_JSON_OK)
	{
		transform.position = pos;

		transform.size.x = tb_json_float((char*)element.value, "{'size'[0", NULL, 0.0f);
		transform.size.y = tb_json_float((char*)element.value, "{'size'[1", NULL, 0.0f);

		transform.z_index = z_index;

		EcsAddDataComponent(&manager->scene->ecs, entity, COMPONENT_TRANSFORM, &transform);
	}

	tb_json_read(json, &element, "{'rigidbody'");
	if (element.error == TB_JSON_OK)
	{
		body.type = (RigidBodyType)tb_json_int((char*)element.value, "{'type'", NULL, 0);

		body.half_size.x = tb_json_float((char*)element.value, "{'halfsize'[0", NULL, 0.0f);
		body.half_size.y = tb_json_float((char*)element.value, "{'halfsize'[1", NULL, 0.0f);

		body.velocity.x = 0.0f;
		body.velocity.y = 0.0f;

		body.offset.x = tb_json_float((char*)element.value, "{'offset'[0", NULL, 0.0f);
		body.offset.y = tb_json_float((char*)element.value, "{'offset'[1", NULL, 0.0f);

		body.position = vec2_add(pos, body.offset);

		body.collides_bottom = 0;
		body.collides_top = 0;
		body.collides_left = 0;
		body.collides_right = 0;

		EcsAddDataComponent(&manager->scene->ecs, entity, COMPONENT_RIGID_BODY, &body);
	}

	tb_json_read(json, &element, "{'sprite'");
	if (element.error == TB_JSON_OK)
	{
		sprite.flip = SPRITE_FLIP_NONE;
		sprite.width = tb_json_float((char*)element.value, "{'size'[0", NULL, -1.0f);
		sprite.height = tb_json_float((char*)element.value, "{'size'[1", NULL, -1.0f);

		if (sprite.width < 0.0f) sprite.width = transform.size.x;
		if (sprite.height < 0.0f) sprite.height = transform.size.y;

		sprite.frame = tb_json_int((char*)element.value, "{'frame'", NULL, 0);

		char texture[APPLICATION_STR_LEN];
		tb_json_string((char*)element.value, "{'texture'", texture, APPLICATION_STR_LEN, NULL);

		sprite.texture = ResourceManagerGetTexture2D(manager->resources, texture);

		if (sprite.texture)
			EcsAddDataComponent(&manager->scene->ecs, entity, COMPONENT_SPRITE, &sprite);
		else
			DEBUG_ERROR("[Scenes] Found sprite but couldn't find texture");

		ZIndex index;
		index.entity = entity;
		index.z_index = z_index;
		EcsAddOrderComponent(&manager->scene->ecs, COMPONENT_Z_INDEX, &index);
	}

	/* ------------------------------------------------------------------ */
	tb_json_read(json, &element, "{'animation'");
	if (element.error == TB_JSON_OK)
	{
		Animator animator;

		animator.current = NULL;

		clib_dict_alloc(&animator.animations, 0);
		for (int i = 0; i < element.elements; i++)
		{
			char anim_name[APPLICATION_STR_LEN];
			tb_json_string((char*)element.value, "{*", anim_name, APPLICATION_STR_LEN, &i);

			tb_json_element anim;
			tb_json_read_format((char*)element.value, &anim, "{'%s'", anim_name);

			unsigned int start = tb_json_int((char*)anim.value, "{'start'", NULL, 0);
			unsigned int length = tb_json_int((char*)anim.value, "{'length'", NULL, 0);
			float delay = tb_json_float((char*)anim.value, "{'delay'", NULL, 0.0f);

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

			AnimatorAddAnimation(&animator, anim_name, animation);
		}
		EcsAddDataComponent(&manager->scene->ecs, entity, COMPONENT_ANIMATION, &animator);
	}

	tb_json_read(json, &element, "{'movement'");
	if (element.error == TB_JSON_OK)
	{
		Movement comp;

		comp.direction = MOVEMENT_LEFT;
		comp.speed = tb_json_float((char*)element.value, "{'speed'", NULL, 0.0f);
		comp.jump_power = tb_json_float((char*)element.value, "{'jumppower'", NULL, 0.0f);

		EcsAddDataComponent(&manager->scene->ecs, entity, COMPONENT_MOVEMENT, &comp);
	}

	tb_json_read(json, &element, "{'camera'");
	if (element.error == TB_JSON_OK)
	{
		CameraController comp;
		comp.camera = manager->scene->camera;
		comp.smooth = tb_json_float((char*)element.value, "{'smooth'", NULL, 0.0f);
		comp.scene_w = manager->scene->width;
		comp.scene_h = manager->scene->height;

		EcsAddDataComponent(&manager->scene->ecs, entity, COMPONENT_CAMERA, &comp);
	}

	tb_json_read(json, &element, "{'interaction'");
	if (element.error == TB_JSON_OK)
	{
		Interaction comp;
		comp.radius = tb_json_float((char*)element.value, "{'radius'", NULL, 0.0f);
		comp.type = (InteractionType)tb_json_int((char*)element.value, "{'type'", NULL, 0);

		EcsAddDataComponent(&manager->scene->ecs, entity, COMPONENT_INTERACTION, &comp);
	}

	tb_json_read(json, &element, "{'interactor'");
	if (element.error == TB_JSON_OK)
	{
		Interactor comp;
		comp.type = (InteractionType)tb_json_int((char*)element.value, "{'type'", NULL, 0);

		EcsAddDataComponent(&manager->scene->ecs, entity, COMPONENT_INTERACTOR, &comp);
	}

	free(json);

	return 1;
}