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

	for (EcsListNode* it = EcsGetComponentList(&scene->ecs, COMPONENT_TEMPLATE)->first; it; it = EcsComponentNodeNext(it))
	{
		tb_jwrite_array_array(&jwc);

		tb_jwrite_set_style(&jwc, TB_JWRITE_INLINE);

		Template* templ = EcsComponentNodeComponent(it);

		/* template */
		tb_jwrite_array_string(&jwc, templ->templ);

		/* pos */
		vec2 pos = GetEntityPosition(&scene->ecs, EcsComponentNodeEntity(it));

		tb_jwrite_array_array(&jwc);
		tb_jwrite_array_float(&jwc, pos.x);
		tb_jwrite_array_float(&jwc, pos.y);
		tb_jwrite_end(&jwc);

		/* z_index */
		tb_jwrite_array_int(&jwc, EntityGetZIndex(&scene->ecs, EcsComponentNodeEntity(it)));

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
	const char* path = SceneGetTemplatePath(scene, templ);
	char* json = ignisReadFile(path, NULL);

	if (!json)
	{
		DEBUG_WARN("[Scenes] Couldn't read template (%s)\n", path);
		return 0;
	}

	/* Components */
	TemplateLoad(scene, entity, templ);
	EntityStateLoad(scene, entity, ENTITY_STATE_NULL);

	TransformLoad(scene, entity, pos, json);
	RigidBodyLoad(scene, entity, json);
	SpriteLoad(scene, entity, z_index, json);
	AnimatorLoad(scene, entity, json);
	MovementLoad(scene, entity, json);
	CameraControllerLoad(scene, entity, json);
	PlayerLoad(scene, entity, json);
	InventoryLoad(scene, entity, json);
	InteractableLoad(scene, entity, json);

	free(json);

	return 1;
}

int SceneLoadMap(Scene* scene, char* json)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'map'");

	size_t width = tb_json_long(element.value, "{'size'[0", NULL, 0);
	size_t height = tb_json_long(element.value, "{'size'[1", NULL, 0);

	float tile_size = tb_json_float(element.value, "{'tile_size'", NULL, 0.0f);

	if (width == 0 || height == 0 || tile_size <= 0.0f)
	{
		DEBUG_ERROR("[Scenes] Map size or tile size can not be zero.");
		return 0;
	}

	tb_json_element tiles;
	tb_json_read(element.value, &tiles, "{'tiles'");

	TileID* tile_ids = NULL;

	if (tiles.error == TB_JSON_OK && tiles.data_type == TB_JSON_ARRAY)
	{
		tile_ids = malloc(tiles.elements * sizeof(TileID));

		if (!tile_ids) return 0;

		tb_json_element id;
		char* value = tiles.value;
		for (size_t i = 0; i < tiles.elements; ++i)
		{
			value = tb_json_array_step(value, &id);
			tb_json_atoi(id.value, tile_ids + i);
		}
	}

	TileType types[] =
	{
		[0] = TILE_EMPTY,
		[1] = TILE_SOLID,
		[2] = TILE_SOLID,
		[8] = TILE_SLOPE_RIGHT,
		[9] = TILE_PLATFORM,
		[10] = TILE_SLOPE_LEFT,
	};

	TileMapLoad(&scene->map, tile_ids, width, height, tile_size, types, 11);
	TileRendererBindMap(&scene->renderer, &scene->map);

	free(tile_ids);

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
