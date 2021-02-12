#include "Scene.h"

#include "Application/Debugger.h"
#include "toolbox/tb_json.h"

#include "SceneLoader.h"
#include "SceneSaver.h"

#include "toolbox/tb_file.h"

int SceneInit(Scene* scene, vec2 size, int (*load)(Ecs* ecs))
{
	if (tb_hashmap_alloc(&scene->scenes, tb_hash_string, tb_hashmap_str_cmp, 0) != TB_HASHMAP_OK)
		return 0;

	tb_hashmap_set_key_alloc_funcs(&scene->scenes, tb_hashmap_str_alloc, tb_hashmap_str_free);
	tb_hashmap_set_value_alloc_funcs(&scene->scenes, tb_hashmap_str_alloc, tb_hashmap_str_free);

	ResourcesInit(&scene->res);

	CameraCreateOrtho(&scene->camera, size.x * .5f, size.y * .5f, 0.0f, size.x, size.y);
	scene->gravity = vec2_zero();

	TileRendererInit(&scene->renderer);

	EcsInit(&scene->ecs);
	return load ? load(&scene->ecs) : 1;
}

int SceneLoadScenes(Scene* scene, const char* reg, const char* start)
{
	char* json = tb_file_read(reg, "rb", NULL);

	if (!json)
	{
		DEBUG_ERROR("[Scenes] Failed to read register (%s)\n", reg);
		return 0;
	}

	tb_json_element element;
	tb_json_read(json, &element, NULL);
	for (int i = 0; i < element.elements; i++)
	{
		char name[APPLICATION_STR_LEN];
		tb_json_string(element.value, "{*", name, APPLICATION_STR_LEN, &i);

		tb_json_element scene_element;
		tb_json_read_format(element.value, &scene_element, "{'%s'", name);

		char scene_path[APPLICATION_PATH_LEN];
		strncpy(scene_path, scene_element.value, scene_element.bytelen);

		scene_path[scene_element.bytelen] = '\0';

		if (!tb_hashmap_insert(&scene->scenes, name, scene_path))
			DEBUG_WARN("[Scenes] Failed to add scene: %s (%s)\n", name, scene_path);
	}

	free(json);

	SceneClearActive(scene);
	SceneChangeActive(scene, start, 0);

	return 1;
}

void SceneDestroy(Scene* scene)
{
	SceneClearActive(scene);

	TileRendererDestroy(&scene->renderer);
	TileMapDestroy(&scene->map);

	EcsDestroy(&scene->ecs);

	ResourcesDestroy(&scene->res);
	tb_hashmap_free(&scene->scenes);
}

void SceneChangeActive(Scene* scene, const char* name, int reload)
{
	if (strcmp(scene->name, name) != 0 || reload)
	{
		/* Check if scene is in the register */
		char* path = tb_hashmap_find(&scene->scenes, name);
		if (!path)
		{
			DEBUG_ERROR("[Scenes] Couldn't find scene: %s\n", name);
			return;
		}

		/* Clear old Scene */
		if (scene->name[0] != '\0') SceneClearActive(scene);

		/* Enter new scene */
		if (!SceneLoad(scene, path))
		{
			DEBUG_ERROR("[Scenes] Failed to load new scene: %s\n", name);
			return;
		}
		strncpy(scene->name, name, APPLICATION_STR_LEN);
	}
}

void SceneClearActive(Scene* scene)
{
	BackgroundFree(&scene->background);
	TileRendererClear(&scene->renderer);

	EcsClear(&scene->ecs);

	ResourcesClear(&scene->res);

	ignisDeleteTexture2D(&scene->item_atlas);
	ignisDeleteTexture2D(&scene->tile_set);
}

void SceneOnEvent(Scene* scene, Event e)
{
	if (EventCheckType(&e, EVENT_WINDOW_RESIZE))
		CameraSetProjectionOrtho(&scene->camera, (float)e.window.width, (float)e.window.height);
}

void SceneOnUpdate(Scene* scene, float deltatime)
{
	BackgroundUpdate(&scene->background, scene->camera.position.x - scene->camera.size.x * .5f, deltatime);

	EcsOnUpdate(&scene->ecs, scene, deltatime);
}

void SceneOnRender(Scene* scene)
{
	BackgroundRender(&scene->background, CameraGetViewProjectionPtr(&scene->camera));

	TileMapRender(&scene->renderer, &scene->tile_set, scene->camera.viewProjection);

	EcsOnRender(&scene->ecs, ECS_RENDER_STAGE_PRIMARY, scene, CameraGetViewProjectionPtr(&scene->camera));
}

void SceneOnRenderUI(Scene* scene)
{
	EcsOnRender(&scene->ecs, ECS_RENDER_STAGE_UI, scene, CameraGetProjectionPtr(&scene->camera));
}

void SceneOnRenderDebug(Scene* scene)
{
	EcsOnRender(&scene->ecs, ECS_RENDER_STAGE_DEBUG, scene, CameraGetViewProjectionPtr(&scene->camera));
}

float SceneGetWidth(const Scene* scene) { return scene->map.tile_size * scene->map.cols; }
float SceneGetHeight(const Scene* scene) { return scene->map.tile_size * scene->map.rows; }
