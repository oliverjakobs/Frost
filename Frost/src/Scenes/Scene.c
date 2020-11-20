#include "Scene.h"

#include "toolbox/tb_json.h"
#include "toolbox/tb_jwrite.h"

#include "Application/Debugger.h"

#include "Frost/Frost.h"


int SceneInit(Scene* scene, Camera* camera, const char* path, Resources* resources)
{
	if (tb_hashmap_alloc(&scene->scene_register, tb_hash_string, tb_hashmap_str_cmp, 0) != TB_HASHMAP_OK) return 0;
	if (tb_hashmap_alloc(&scene->templates, tb_hash_string, tb_hashmap_str_cmp, 0) != TB_HASHMAP_OK) return 0;

	tb_hashmap_set_key_alloc_funcs(&scene->scene_register, tb_hashmap_str_alloc, tb_hashmap_str_free);
	tb_hashmap_set_value_alloc_funcs(&scene->scene_register, tb_hashmap_str_alloc, tb_hashmap_str_free);
	tb_hashmap_set_key_alloc_funcs(&scene->templates, tb_hashmap_str_alloc, tb_hashmap_str_free);
	tb_hashmap_set_value_alloc_funcs(&scene->templates, tb_hashmap_str_alloc, tb_hashmap_str_free);

	char* json = ignisReadFile(path, NULL);

	if (!json)
	{
		tb_hashmap_free(&scene->scene_register);
		tb_hashmap_free(&scene->templates);
		DEBUG_ERROR("[Scenes] Failed to read register (%s)\n", path);
		return 0;
	}

	tb_json_element element;
	tb_json_read(json, &element, "{'scenes'");
	for (int i = 0; i < element.elements; i++)
	{
		char name[APPLICATION_STR_LEN];
		tb_json_string((char*)element.value, "{*", name, APPLICATION_STR_LEN, &i);

		tb_json_element scene_element;
		tb_json_read_format((char*)element.value, &scene_element, "{'%s'", name);

		char scene_path[APPLICATION_PATH_LEN];
		strncpy(scene_path, (char*)scene_element.value, scene_element.bytelen);

		scene_path[scene_element.bytelen] = '\0';

		if (!tb_hashmap_insert(&scene->scene_register, name, scene_path))
			DEBUG_WARN("[Scenes] Failed to add scene: %s (%s)", name, scene_path);
	}

	tb_json_read(json, &element, "{'templates'");
	for (int i = 0; i < element.elements; i++)
	{
		char name[APPLICATION_STR_LEN];
		tb_json_string((char*)element.value, "{*", name, APPLICATION_STR_LEN, &i);

		tb_json_element template_element;
		tb_json_read_format((char*)element.value, &template_element, "{'%s'", name);

		char templ_path[APPLICATION_PATH_LEN];
		strncpy(templ_path, (char*)template_element.value, template_element.bytelen);

		templ_path[template_element.bytelen] = '\0';

		if (!tb_hashmap_insert(&scene->templates, name, templ_path))
			DEBUG_WARN("[Scenes] Failed to add template: %s (%s)", name, templ_path);
	}

	free(json);

	scene->camera = camera;
	scene->resources = resources;

	SceneClearActive(scene);

	EcsInit(&scene->ecs);

	return 1;
}

void SceneDestroy(Scene* scene)
{
	SceneClearActive(scene);

	EcsDestroy(&scene->ecs);

	tb_hashmap_free(&scene->scene_register);
	tb_hashmap_free(&scene->templates);
}

void SceneChangeActive(Scene* scene, const char* name)
{
	if (strcmp(scene->name, name) != 0)
	{
		/* Check if scene is in the register */
		char* path = tb_hashmap_find(&scene->scene_register, name);
		if (!path)
		{
			DEBUG_ERROR("[Scenes] Couldn't find scene: %s\n", name);
			return;
		}

		/* Clear old Scene */
		if (scene->name[0] != '\0')
			SceneClearActive(scene);

		/* Enter new scene */
		SceneLoad(scene, path);
		strcpy(scene->name, name);
	}
}

void SceneClearActive(Scene* scene)
{
	BackgroundClear(&scene->background);
	EcsClear(&scene->ecs);

	memset(scene->name, '\0', APPLICATION_STR_LEN);
	scene->width = 0.0f;
	scene->height = 0.0f;
}

void SceneOnEvent(Scene* scene, Event e)
{
	EcsOnEvent(&scene->ecs, e);
}

void SceneOnUpdate(Scene* scene, float deltatime)
{
	BackgroundUpdate(&scene->background, scene->camera->position.x - scene->camera->size.x / 2.0f, deltatime);

	EcsOnUpdate(&scene->ecs, deltatime);
}

void SceneOnRender(Scene* scene)
{
	BackgroundRender(&scene->background, CameraGetViewProjectionPtr(scene->camera));

	EcsOnRender(&scene->ecs, ECS_RENDER_STAGE_PRIMARY, CameraGetViewProjectionPtr(scene->camera));
}

void SceneOnRenderDebug(Scene* scene)
{
	EcsOnRender(&scene->ecs, ECS_RENDER_STAGE_DEBUG, CameraGetViewProjectionPtr(scene->camera));
}

int SceneLoad(Scene* scene, const char* path)
{
	if (!scene) return 0;

	char* json = ignisReadFile(path, NULL);

	if (!json)
	{
		DEBUG_ERROR("[Scenes] Couldn't read scene template: %s\n", path);
		return 0;
	}

	float width = tb_json_float(json, "{'size'[0", NULL, 0.0f);
	float height = tb_json_float(json, "{'size'[1", NULL, 0.0f);

	if (width <= 0.0f || height <= 0.0f)
	{
		DEBUG_ERROR("[Scenes] Failed to load scene (wrong size): %s", json);
		free(json);
		return 0;
	}

	/* set scene data */
	scene->width = width;
	scene->height = height;

	tb_json_element element;
	tb_json_read(json, &element, "{'background'");

	if (element.error == TB_JSON_OK && element.data_type == TB_JSON_ARRAY)
	{
		char* value = element.value;
		Background* background = &scene->background;

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

			BackgroundPushLayer(background, ResourcesGetTexture2D(scene->resources, name), x, y, w, h, parallax);
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
			if (!SceneLoadTemplate(scene, templ, EcsEntityGetNextID(), pos, z_index))
			{
				DEBUG_ERROR("[Scenes] Failed to load template %s\n", templ);
			}
		}
	}

	free(json);
	return 1;
}

int SceneSave(Scene* scene, const char* path)
{
	char* temp_ext = ".temp";
	char* temp_path = malloc(strlen(path) + strlen(temp_ext));

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
		tb_jwrite_array_string(&jwc, ResourcesGetTexture2DName(scene->resources, bg->texture));

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

	for (size_t i = 0; i < EcsGetComponentList(&scene->ecs, COMPONENT_TEMPLATE)->list.used; ++i)
	{
		tb_jwrite_array_array(&jwc);

		tb_jwrite_set_style(&jwc, TB_JWRITE_INLINE);

		Template* templ = EcsGetOrderComponent(&scene->ecs, i, COMPONENT_TEMPLATE);

		/* template */
		tb_jwrite_array_string(&jwc, templ->templ);

		/* pos */
		vec2 pos = GetEntityPosition(&scene->ecs, templ->entity);

		tb_jwrite_array_array(&jwc);
		tb_jwrite_array_float(&jwc, pos.x);
		tb_jwrite_array_float(&jwc, pos.y);
		tb_jwrite_end(&jwc);

		/* z_index */
		ZIndex* indexed = EcsComponentListFind(EcsGetComponentList(&scene->ecs, COMPONENT_Z_INDEX), templ->entity);
		tb_jwrite_array_int(&jwc, indexed ? indexed->z_index : 0);

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

int SceneLoadTemplate(Scene* scene, const char* templ, EcsEntityID entity, vec2 pos, int z_index)
{
	char* path = tb_hashmap_find(&scene->templates, templ);
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

	Template t;
	t.entity = entity;
	t.templ = malloc(strlen(templ));
	strcpy(t.templ, templ);
	EcsAddOrderComponent(&scene->ecs, COMPONENT_TEMPLATE, &t);
	/* TODO: Free template? */

	ZIndex index;
	index.entity = entity;
	index.z_index = z_index;
	EcsAddOrderComponent(&scene->ecs, COMPONENT_Z_INDEX, &index);
	/* TODO: ZIndex only for entities with sprite component */

	/* Components */
	tb_json_read(json, &element, "{'transform'");
	if (element.error == TB_JSON_OK)
	{
		Transform transform;
		transform.position = pos;

		transform.size.x = tb_json_float((char*)element.value, "{'size'[0", NULL, 0.0f);
		transform.size.y = tb_json_float((char*)element.value, "{'size'[1", NULL, 0.0f);

		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_TRANSFORM, &transform);
	}

	/* ------------------------------------------------------------------ */
	/* TODO to code generation */
	RigidBodyLoad(scene, entity, json);
	SpriteLoad(scene, entity, json);
	AnimatorLoad(scene, entity, json);
	MovementLoad(scene, entity, json);
	CameraControllerLoad(scene, entity, json);
	InventoryLoad(scene, entity, json);
	InteractorLoad(scene, entity, json);
	InteractionLoad(scene, entity, json);

	free(json);

	return 1;
}