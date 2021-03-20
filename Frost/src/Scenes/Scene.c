#include "Scene.h"

#include "Application/Logger.h"

#include "toolbox/tb_str.h"

int SceneInit(Scene* scene, vec2 size, SceneLoadFn load, SceneSaveFn save)
{
	ResourcesInit(&scene->res);

	CameraCreateOrtho(&scene->camera, 0.0f, 0.0f, 0.0f, size.x, size.y);
	scene->gravity = vec2_zero();

	TileRendererInit(&scene->renderer);

	scene->load = load;
	scene->save = save;

	EcsInit(&scene->ecs);
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
	if (strcmp(scene->path, path) != 0 || reload)
	{
		/* Clear and save old Scene */
		if (scene->path[0] != '\0')
		{
			if (scene->save && !scene->save(scene, scene->path))
				DEBUG_ERROR("[Scenes] Failed to save scene: %s", scene->path);
			SceneClearActive(scene);
		}

		/* Enter new scene */
		if (scene->load && !scene->load(scene, path))
			DEBUG_ERROR("[Scenes] Failed to load new scene: %s", path);

		tb_strlcpy(scene->path, path, APPLICATION_PATH_LEN);
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
