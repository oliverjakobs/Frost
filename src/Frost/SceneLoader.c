#include "SceneLoader.h"

#include "Memory.h"
#include "FrostParser.h"

#include "toolbox/tb_file.h"

static int SceneLoadFromFile(Scene* scene, const char* path, SceneLoadError(*load_func)(Scene*, char*))
{
	char* json = tb_file_read_alloc(path, "rb", FrostMalloc, FrostFree);
	if (!json)
	{
		MINIMAL_ERROR("[Scenes] Failed to read file (%s).", path);
		return 0;
	}

	SceneLoadError error = load_func(scene, json);
	FrostFree(json);

	if (error != SCENE_LOAD_OK)
	{
		MINIMAL_ERROR("[Scenes] %s.\n", SceneLoadErrorDesc(error));
		return 0;
	}

	return 1;
}

static int SceneLoadTexture2D(char* json, IgnisTexture2D* texture)
{
	char path[APPLICATION_PATH_LEN];
	tb_json_string(json, "{'path'", path, APPLICATION_PATH_LEN, NULL);

	int rows = tb_json_int(json, "{'atlas'[0", NULL, 1);
	int cols = tb_json_int(json, "{'atlas'[1", NULL, 1);

	return ignisCreateTexture2D(texture, path, rows, cols, 1, NULL);
}

int SceneLoad(Scene* scene, const char* path)
{
	if (!scene) return 0;

	char* json = tb_file_read_alloc(path, "rb", FrostMalloc, FrostFree);

	if (!json)
	{
		MINIMAL_ERROR("[Scenes] Couldn't read scene template: %s", path);
		return 0;
	}

	/* load map */
	char map_path[APPLICATION_PATH_LEN];
	tb_json_string(json, "{'map'", map_path, APPLICATION_PATH_LEN, NULL);

	if (!SceneLoadFromFile(scene, map_path, SceneLoadMap))
	{
		FrostFree(json);
		return 0;
	}

	scene->gravity.x = tb_json_float(json, "{'gravity'[0", NULL, 0.0f);
	scene->gravity.y = tb_json_float(json, "{'gravity'[1", NULL, 0.0f);

	/* load item atlas */
	tb_json_element element;
	tb_json_read(json, &element, "{'item_atlas'");
	if (!SceneLoadTexture2D(element.value, &scene->item_atlas))
		MINIMAL_WARN("[Scenes] Could not load item atlas.");

	/* load tile set */
	tb_json_read(json, &element, "{'tile_set'");
	if (!SceneLoadTexture2D(element.value, &scene->tile_set)) 
		MINIMAL_WARN("[Scenes] Could not load item atlas.");

	/* load textures */
	tb_json_read(json, &element, "{'textures'");
	for (int i = 0; i < element.elements; i++)
	{
		char name[APPLICATION_STR_LEN];
		tb_json_string(element.value, "{*", name, APPLICATION_STR_LEN, &i);

		tb_json_element texture;
		tb_json_read_format(element.value, &texture, "{'%s'", name);

		char path[APPLICATION_PATH_LEN];
		tb_json_string(texture.value, "{'path'", path, APPLICATION_PATH_LEN, NULL);

		int rows = tb_json_int(texture.value, "{'atlas'[0", NULL, 1);
		int cols = tb_json_int(texture.value, "{'atlas'[1", NULL, 1);

		ResourcesAddTexture2D(&scene->res, name, path, rows, cols);
	}

	/* load background */
	tb_json_read(json, &element, "{'background'");
	if (element.error == TB_JSON_OK && element.data_type == TB_JSON_ARRAY)
	{
		char* value = element.value;
		scene->background = BackgroundInit(element.elements);
		for (int i = 0; i < element.elements; i++)
		{
			tb_json_element layer;
			value = tb_json_array_step(value, &layer);

			char path[APPLICATION_PATH_LEN];
			tb_json_string(layer.value, "[0", path, APPLICATION_PATH_LEN, NULL);

			IgnisTexture2D texture;
			if (!ignisCreateTexture2D(&texture, path, 1, 1, 1, NULL))
			{
				MINIMAL_WARN("Failed to load background layer %d (%s)", i, path);
				continue;
			}

			float x = tb_json_float(layer.value, "[1", NULL, 0.0f);
			float y = tb_json_float(layer.value, "[2", NULL, 0.0f);

			float w = tb_json_float(layer.value, "[3", NULL, 0.0f);
			float h = tb_json_float(layer.value, "[4", NULL, 0.0f);

			float parallax = tb_json_float(layer.value, "[5", NULL, 0.0f);

			scene->background = BackgroundPushLayer(scene->background, texture, x, y, w, h, parallax);
		}
	}

	/* load save */
	char save_path[APPLICATION_PATH_LEN];
	tb_json_string(json, "{'save'", save_path, APPLICATION_PATH_LEN, NULL);

	FrostFree(json);

	return SceneLoadFromFile(scene, save_path, SceneLoadSaveState);
}

int SceneLoadTemplate(Scene* scene, const char* templ, vec2 pos, int z_index, int variant)
{
	char json[FROST_TEMPLATE_SIZE];
	size_t size = tb_file_read_buf(templ, "rb", json, FROST_TEMPLATE_SIZE);
	if (size == FROST_TEMPLATE_SIZE)
		MINIMAL_WARN("[Scenes] Template buffer may be too small");

	if (size == 0)
	{
		MINIMAL_WARN("[Scenes] Couldn't read template (%s)", templ);
		return 0;
	}

	EcsEntityID entity = EcsEntityGetNextID();

	/* Load components */
	TemplateLoad(&scene->ecs, entity, templ);
	EntityStateLoad(&scene->ecs, entity, ENTITY_STATE_NULL);

	TransformLoad(json, &scene->ecs, entity, pos);
	RigidBodyLoad(json, &scene->ecs, entity);
	SpriteLoad(json, &scene->ecs, entity, &scene->res, z_index, variant);
	AnimatorLoad(json, &scene->ecs, entity);
	MovementLoad(json, &scene->ecs, entity);
	CameraControllerLoad(json, &scene->ecs, entity, scene);
	PlayerLoad(json, &scene->ecs, entity);
	InventoryLoad(json, &scene->ecs, entity, scene->camera.size);
	InteractableLoad(json, &scene->ecs, entity);

	return 1;
}

SceneLoadError SceneLoadTextures(Scene* scene, char* json)
{

	return SCENE_LOAD_OK;
}

static void* SceneStreamTiles(void* stream, TileID* id)
{
	tb_json_element element;
	stream = tb_json_array_step(stream, &element);
	tb_json_atoi(element.value, id);
	return stream;
}

static void* SceneStreamTileTypes(void* stream, TileType* type)
{
	tb_json_element element;
	stream = tb_json_array_step(stream, &element);
	*type = FrostParseTileType(element.value, element.bytelen);
	return stream;
}

SceneLoadError SceneLoadMap(Scene* scene, char* json)
{
	size_t cols = tb_json_long(json, "{'size'[0", NULL, 0);
	size_t rows = tb_json_long(json, "{'size'[1", NULL, 0);

	float tile_size = tb_json_float(json, "{'tile_size'", NULL, 0.0f);

	if (!TileMapLoad(&scene->map, rows, cols, tile_size, scene->allocator)) return SCENE_LOAD_MAP_ERROR;

	tb_json_element types;
	tb_json_read(json, &types, "{'tile_types'");

	if (!tb_json_is_type(&types, TB_JSON_ARRAY) || 
		!TileMapStreamTypes(&scene->map, types.value, SceneStreamTileTypes, types.elements))
	{
		TileMapDestroy(&scene->map);
		return SCENE_LOAD_STREAM_TYPES_ERROR;
	}

	tb_json_element tiles;
	tb_json_read(json, &tiles, "{'tiles'");

	if (!tb_json_is_type(&tiles, TB_JSON_ARRAY) || 
		!TileMapStreamTiles(&scene->map, tiles.value, SceneStreamTiles, tiles.elements))
	{
		TileMapDestroy(&scene->map);
		return SCENE_LOAD_STREAM_MAP_ERROR;
	}

	TileRendererBindMap(&scene->renderer, &scene->map);

	TileMapSetBorder(&scene->map, TILE_BOTTOM, 1);
	TileMapSetBorder(&scene->map, TILE_LEFT, 1);
	TileMapSetBorder(&scene->map, TILE_RIGHT, 1);

	return SCENE_LOAD_OK;
}

SceneLoadError SceneLoadSaveState(Scene* scene, char* json)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'templates'");
	if (element.error == TB_JSON_OK && element.data_type == TB_JSON_ARRAY)
	{
		char* value = element.value;

		for (size_t i = 0; i < element.elements; ++i)
		{
			tb_json_element entity_template;
			value = tb_json_array_step(value, &entity_template);

			char templ[APPLICATION_PATH_LEN];
			tb_json_string(entity_template.value, "[0", templ, APPLICATION_PATH_LEN, NULL);

			vec2 pos;
			pos.x = tb_json_float(entity_template.value, "[1[0", NULL, 0.0f);
			pos.y = tb_json_float(entity_template.value, "[1[1", NULL, 0.0f);

			int z_index = tb_json_int(entity_template.value, "[2", NULL, 0);
			int variant = tb_json_int(entity_template.value, "[3", NULL, 0);

			/* Load Template */
			if (!SceneLoadTemplate(scene, templ, pos, z_index, variant))
			{
				MINIMAL_ERROR("[Scenes] Failed to load template %s", templ);
			}
		}
	}

	return SCENE_LOAD_OK;
}

const char* SceneLoadErrorDesc(SceneLoadError error)
{
	switch (error)
	{
	case SCENE_LOAD_OK:					return "No error";
	case SCENE_LOAD_READ_ERROR:			return "Failed to read file";
	case SCENE_LOAD_MAP_ERROR:			return "Failed to load map";
	case SCENE_LOAD_MAP_SIZE_ERROR:		return "Map size or tile size can not be zero";
	case SCENE_LOAD_STREAM_MAP_ERROR:	return "Failed to stream tiles";
	case SCENE_LOAD_STREAM_TYPES_ERROR: return "Failed to stream tile types";
	default:							return "Unkown error";
	}
}
