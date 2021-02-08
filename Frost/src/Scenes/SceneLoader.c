#include "SceneLoader.h"

#include "Application/Debugger.h"

#include "toolbox/tb_json.h"
#include "toolbox/tb_jwrite.h"

#include "Frost/Frost.h"

static int SceneLoadMap(Scene* scene, char* json);
static int SceneSaveMap(Scene* scene, tb_jwrite_control* jwc);

int SceneLoad(Scene* scene, const char* path)
{
	if (!scene) return 0;

	char* json = ignisReadFile(path, NULL);

	if (!json)
	{
		DEBUG_ERROR("[Scenes] Couldn't read scene template: %s\n", path);
		return 0;
	}

	if (!SceneLoadMap(scene, json))
	{
		DEBUG_ERROR("[Scenes] Failed to load scene: %s", path);
		free(json);
		return 0;
	}

	scene->gravity.x = tb_json_float(json, "{'gravity'[0", NULL, 0.0f);
	scene->gravity.y = tb_json_float(json, "{'gravity'[1", NULL, 0.0f);

	tb_json_element element;
	tb_json_read(json, &element, "{'background'");
	if (element.error == TB_JSON_OK && element.data_type == TB_JSON_ARRAY)
	{
		char* value = element.value;
		Background* background = &scene->background;

		BackgroundAlloc(background, element.elements);
		for (int i = 0; i < element.elements; i++)
		{
			tb_json_element entity;
			value = tb_json_array_step(value, &entity);

			char name[APPLICATION_STR_LEN];
			tb_json_string(entity.value, "[0", name, APPLICATION_STR_LEN, NULL);

			float x = tb_json_float(entity.value, "[1", NULL, 0.0f);
			float y = tb_json_float(entity.value, "[2", NULL, 0.0f);

			float w = tb_json_float(entity.value, "[3", NULL, 0.0f);
			float h = tb_json_float(entity.value, "[4", NULL, 0.0f);

			float parallax = tb_json_float(entity.value, "[5", NULL, 0.0f);

			BackgroundPushLayer(background, ResourcesGetTexture2D(scene->resources, name), x, y, w, h, parallax);
		}
	}

	tb_json_read(json, &element, "{'templates'");

	if (element.error == TB_JSON_OK && element.data_type == TB_JSON_ARRAY)
	{
		char* value = element.value;

		for (size_t i = 0; i < element.elements; ++i)
		{
			tb_json_element entity_template;
			value = tb_json_array_step(value, &entity_template);

			char templ[APPLICATION_STR_LEN];
			tb_json_string(entity_template.value, "[0", templ, APPLICATION_STR_LEN, NULL);

			vec2 pos;
			pos.x = tb_json_float(entity_template.value, "[1[0", NULL, 0.0f);
			pos.y = tb_json_float(entity_template.value, "[1[1", NULL, 0.0f);

			int z_index = tb_json_int(entity_template.value, "[2", NULL, 0);
			int variant = tb_json_int(entity_template.value, "[3", NULL, 0);

			/* Load Template */
			if (!SceneLoadTemplate(scene, templ, pos, z_index, variant))
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

	/* background */
	tb_jwrite_set_style(&jwc, TB_JWRITE_NEWLINE);
	tb_jwrite_array(&jwc, "background");

	for (int i = 0; i < scene->background.layer_count; ++i)
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

	EcsList* list = EcsGetComponentList(&scene->ecs, COMPONENT_TEMPLATE);
	for (EcsListNode* it = list->first; it; it = EcsListNodeNext(it))
	{
		tb_jwrite_array_array(&jwc);

		tb_jwrite_set_style(&jwc, TB_JWRITE_INLINE);

		Template* templ = EcsListNodeComponent(it);

		/* template */
		tb_jwrite_array_string(&jwc, templ->templ);

		/* pos */
		vec2 pos = GetEntityPosition(&scene->ecs, EcsListNodeEntity(it));

		tb_jwrite_array_array(&jwc);
		tb_jwrite_array_float(&jwc, pos.x);
		tb_jwrite_array_float(&jwc, pos.y);
		tb_jwrite_end(&jwc);

		/* z_index */
		tb_jwrite_array_int(&jwc, EntityGetZIndex(&scene->ecs, EcsListNodeEntity(it)));

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

int SceneLoadTemplate(Scene* scene, const char* templ, vec2 pos, int z_index, int variant)
{
	const char* path = SceneGetTemplatePath(scene, templ);
	char* json = ignisReadFile(path, NULL);

	if (!json)
	{
		DEBUG_WARN("[Scenes] Couldn't read template (%s)\n", path);
		return 0;
	}

	EcsEntityID entity = EcsEntityGetNextID();

	/* Load components */
	TemplateLoad(&scene->ecs, entity, templ);
	EntityStateLoad(&scene->ecs, entity, ENTITY_STATE_NULL);

	TransformLoad(json, &scene->ecs, entity, pos);
	RigidBodyLoad(json, &scene->ecs, entity);
	SpriteLoad(json, &scene->ecs, entity, scene->resources, z_index, variant);
	AnimatorLoad(json, &scene->ecs, entity);
	MovementLoad(json, &scene->ecs, entity);
	CameraControllerLoad(json, &scene->ecs, entity, scene);
	PlayerLoad(json, &scene->ecs, entity);
	InventoryLoad(json, &scene->ecs, entity, scene->camera.size);
	InteractableLoad(json, &scene->ecs, entity);

	free(json);

	return 1;
}

void* SceneStreamTiles(void* stream, TileID* id)
{
	tb_json_element element;
	stream = tb_json_array_step(stream, &element);
	tb_json_atoi(element.value, id);
	return stream;
}

int SceneLoadMap(Scene* scene, char* json)
{
	size_t cols = tb_json_long(json, "{'size'[0", NULL, 0);
	size_t rows = tb_json_long(json, "{'size'[1", NULL, 0);

	float tile_size = tb_json_float(json, "{'tile_size'", NULL, 0.0f);

	if (rows == 0 || cols == 0 || tile_size <= 0.0f)
	{
		DEBUG_ERROR("[Scenes] Map size or tile size can not be zero.\n");
		return 0;
	}

	if (!TileMapLoad(&scene->map, rows, cols, tile_size))
	{
		DEBUG_ERROR("[Scenes] Failed to load map.\n");
		return 0;
	}

	/* TODO: stream types */
	TileType types[] =
	{
		[0] = TILE_EMPTY,
		[1] = TILE_SOLID,
		[2] = TILE_SOLID,
		[8] = TILE_SLOPE_RIGHT,
		[9] = TILE_PLATFORM,
		[10] = TILE_SLOPE_LEFT,
	};

	tb_json_element tiles;
	tb_json_read(json, &tiles, "{'tiles'");

	if (tiles.error != TB_JSON_OK || tiles.data_type != TB_JSON_ARRAY)
	{
		DEBUG_ERROR("[Scenes] Failed to read tiles.\n");
		return 0;
	}

	TileMapStreamTiles(&scene->map, tiles.value, SceneStreamTiles, types, 11);
	TileRendererBindMap(&scene->renderer, &scene->map);

	scene->tile_set = ResourcesGetTexture2D(scene->resources, "tiles");

	TileMapSetBorder(&scene->map, TILE_BOTTOM, 1);
	TileMapSetBorder(&scene->map, TILE_LEFT, 1);
	TileMapSetBorder(&scene->map, TILE_RIGHT, 1);

	return 1;
}

/* TODO: implement map saving */
int SceneSaveMap(Scene* scene, tb_jwrite_control* jwc)
{
	return 0;
}
