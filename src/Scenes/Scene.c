#include "Scene.h"

// #include "Minimal/Application.h"

#include "toolbox/tb_str.h"

int SceneInit(Scene* scene, vec2 size, SceneLoadFn load, SceneSaveFn save, tb_allocator* allocator)
{
	ResourcesInit(&scene->res, allocator);
	scene->item_atlas = NULL;
	scene->tile_set = NULL;

	CameraCreateOrtho(&scene->camera, 0.0f, 0.0f, size.x, size.y);
	scene->gravity = vec2_zero();

	TileRendererInit(&scene->renderer);

	scene->load = load;
	scene->save = save;
	scene->allocator = allocator;

	memset(scene->path, '\0', APPLICATION_PATH_LEN);

	EcsInit(&scene->ecs);
	EcsMemSetAllocator(allocator, tb_mem_malloc, tb_mem_realloc, tb_mem_free);
	return 1;
}

void SceneDestroy(Scene* scene)
{
	SceneClearActive(scene);

	TileRendererDestroy(&scene->renderer);
	TileMapDestroy(&scene->map);

	EcsDestroy(&scene->ecs);

	ResourcesDestroy(&scene->res);
}

void SceneChangeActive(Scene* scene, const char* path, int reload)
{
	if (!tb_streq(scene->path, path) || reload)
	{
		/* Clear and save old Scene */
		if (scene->path[0] != '\0')
		{
			if (scene->save && !scene->save(scene, scene->path))
				MINIMAL_ERROR("[Scenes] Failed to save scene: %s", scene->path);
			SceneClearActive(scene);
		}

		/* Enter new scene */
		if (scene->load && !scene->load(scene, path))
			MINIMAL_ERROR("[Scenes] Failed to load new scene: %s", path);

		tb_strlcpy(scene->path, path, APPLICATION_PATH_LEN);
	}
}

void SceneClearActive(Scene* scene)
{
	BackgroundDestroy(scene->background);
	scene->background = NULL;

	TileRendererClear(&scene->renderer);

	EcsClear(&scene->ecs);

	ResourcesClear(&scene->res);
}

void SceneOnEvent(Scene* scene, const MinimalEvent* e)
{
	if (MinimalCheckEventType(e, MINIMAL_EVENT_WINDOW_SIZE))
		CameraSetProjectionOrtho(&scene->camera, (float)e->lParam, (float)e->rParam);
}

void SceneOnUpdate(Scene* scene, float deltatime)
{
	BackgroundUpdate(scene->background, scene->camera.position.x - scene->camera.size.x * .5f, deltatime);

	EcsOnUpdate(&scene->ecs, scene, deltatime);
}

void SceneOnRender(Scene* scene)
{
	BackgroundRender(scene->background, CameraGetViewProjectionPtr(&scene->camera));

	TileMapRender(&scene->renderer, scene->tile_set, scene->camera.viewProjection);

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
