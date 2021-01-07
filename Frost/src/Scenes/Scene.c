#include "Scene.h"

#include "Application/Debugger.h"
#include "toolbox/tb_json.h"

#include "SceneLoader.h"

int SceneInit(Scene* scene, Camera* camera, Resources* resources, int (*load)(Ecs* ecs))
{
	scene->camera = camera;
	scene->resources = resources;

	TileRendererInit(&scene->renderer);

	EcsInit(&scene->ecs);
	return load ? load(&scene->ecs) : 1;
}

int SceneLoadScenes(Scene* scene, const char* reg, const char* start)
{
	if (tb_hashmap_alloc(&scene->scene_register, tb_hash_string, tb_hashmap_str_cmp, 0) != TB_HASHMAP_OK) return 0;
	if (tb_hashmap_alloc(&scene->templates, tb_hash_string, tb_hashmap_str_cmp, 0) != TB_HASHMAP_OK) return 0;

	tb_hashmap_set_key_alloc_funcs(&scene->scene_register, tb_hashmap_str_alloc, tb_hashmap_str_free);
	tb_hashmap_set_value_alloc_funcs(&scene->scene_register, tb_hashmap_str_alloc, tb_hashmap_str_free);
	tb_hashmap_set_key_alloc_funcs(&scene->templates, tb_hashmap_str_alloc, tb_hashmap_str_free);
	tb_hashmap_set_value_alloc_funcs(&scene->templates, tb_hashmap_str_alloc, tb_hashmap_str_free);

	char* json = ignisReadFile(reg, NULL);

	if (!json)
	{
		tb_hashmap_free(&scene->scene_register);
		tb_hashmap_free(&scene->templates);
		DEBUG_ERROR("[Scenes] Failed to read register (%s)\n", reg);
		return 0;
	}

	tb_json_element element;
	tb_json_read(json, &element, "{'scenes'");
	for (int i = 0; i < element.elements; i++)
	{
		char name[APPLICATION_STR_LEN];
		tb_json_string(element.value, "{*", name, APPLICATION_STR_LEN, &i);

		tb_json_element scene_element;
		tb_json_read_format(element.value, &scene_element, "{'%s'", name);

		char scene_path[APPLICATION_PATH_LEN];
		strncpy(scene_path, scene_element.value, scene_element.bytelen);

		scene_path[scene_element.bytelen] = '\0';

		if (!tb_hashmap_insert(&scene->scene_register, name, scene_path))
			DEBUG_WARN("[Scenes] Failed to add scene: %s (%s)", name, scene_path);
	}

	tb_json_read(json, &element, "{'templates'");
	for (int i = 0; i < element.elements; i++)
	{
		char name[APPLICATION_STR_LEN];
		tb_json_string(element.value, "{*", name, APPLICATION_STR_LEN, &i);

		tb_json_element template_element;
		tb_json_read_format(element.value, &template_element, "{'%s'", name);

		char templ_path[APPLICATION_PATH_LEN];
		strncpy(templ_path, template_element.value, template_element.bytelen);

		templ_path[template_element.bytelen] = '\0';

		if (!tb_hashmap_insert(&scene->templates, name, templ_path))
			DEBUG_WARN("[Scenes] Failed to add template: %s (%s)", name, templ_path);
	}

	free(json);

	SceneClearActive(scene);
	SceneChangeActive(scene, start);

	return 1;
}

void SceneDestroy(Scene* scene)
{
	SceneClearActive(scene);

	TileRendererDestroy(&scene->renderer);
	TileMapDestroy(&scene->map);

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
		strncpy(scene->name, name, APPLICATION_STR_LEN);
	}
}

void SceneClearActive(Scene* scene)
{
	BackgroundFree(&scene->background);
	TileRendererClear(&scene->renderer);

	EcsClear(&scene->ecs);

	memset(scene->name, '\0', APPLICATION_STR_LEN);
}

void SceneOnEvent(Scene* scene, Event e)
{

}

void SceneOnUpdate(Scene* scene, float deltatime)
{
	BackgroundUpdate(&scene->background, scene->camera->position.x - scene->camera->size.x / 2.0f, deltatime);

	EcsOnUpdate(&scene->ecs, deltatime);
}

void SceneOnRender(Scene* scene)
{
	BackgroundRender(&scene->background, CameraGetViewProjectionPtr(scene->camera));

	TileMapRender(&scene->renderer, scene->tile_set, scene->camera->viewProjection);

	EcsOnRender(&scene->ecs, ECS_RENDER_STAGE_PRIMARY, CameraGetViewProjectionPtr(scene->camera));
}

void SceneOnRenderUI(Scene* scene)
{
	EcsOnRender(&scene->ecs, ECS_RENDER_STAGE_UI, CameraGetProjectionPtr(scene->camera));
}

void SceneOnRenderDebug(Scene* scene)
{
	EcsOnRender(&scene->ecs, ECS_RENDER_STAGE_DEBUG, CameraGetViewProjectionPtr(scene->camera));
}

const char* SceneGetTemplatePath(const Scene* scene, const char* templ)
{
	const char* path = tb_hashmap_find(&scene->templates, templ);
	if (!path) DEBUG_WARN("[Scenes] Couldn't find template for %s\n", templ);
	return path;
}

float SceneGetWidth(const Scene* scene) { return scene->map.tile_size * scene->map.width; }
float SceneGetHeight(const Scene* scene) { return scene->map.tile_size * scene->map.height; }
