#include "SceneManager.h"

#include "json/tb_json.h"
#include "json/tb_jwrite.h"

#include "Application/Debugger.h"

#include "SceneLoader.h"

int SceneManagerInit(SceneManager* manager, const char* reg, Resources* resources, Camera* camera, float gridsize, uint16_t padding)
{
	manager->resources = resources;
	manager->camera = camera;

	memset(manager->scene_name, '\0', APPLICATION_STR_LEN);
	manager->width = 0.0f;
	manager->height = 0.0f;

	clib_strmap_alloc(&manager->scenes, 0);
	clib_strmap_alloc(&manager->templates, 0);

	ScenesLoadRegister(manager, reg);

	AnimationConditionsInit();
	EcsInit(&manager->ecs);

	return 1;
}

void SceneManagerDestroy(SceneManager* manager)
{
	if (manager->scene_name[0] != '\0')
		SceneManagerClearActive(manager);

	AnimationConditionsDestroy();

	clib_strmap_free(&manager->scenes);
	clib_strmap_free(&manager->templates);

	EcsDestroy(&manager->ecs);
}

void SceneManagerChangeActive(SceneManager* manager, const char* name)
{
	if (strcmp(manager->scene_name, name) != 0)
	{
		/* Check if scene is in the register */
		char* path = clib_strmap_find(&manager->scenes, name);
		if (!path)
		{
			DEBUG_ERROR("[SceneManager] Couldn't find scene: %s\n", name);
			return;
		}

		/* Clear old Scene */
		if (manager->scene_name[0] != '\0')
			SceneManagerClearActive(manager);

		/* Enter new scene */
		ScenesLoadScene(manager, path);
		strcpy(manager->scene_name, name);
	}
}

void SceneManagerClearActive(SceneManager* manager)
{
	BackgroundClear(&manager->background);
	EcsClear(&manager->ecs);

	memset(manager->scene_name, '\0', APPLICATION_STR_LEN);
	manager->width = 0;
	manager->height = 0;
}

void SceneManagerOnEvent(SceneManager* manager, Event e)
{

}

void SceneManagerOnUpdate(SceneManager* manager, float deltatime)
{
	BackgroundUpdate(&manager->background, manager->camera->position.x - manager->camera->size.x / 2.0f, deltatime);

	EcsUpdate(&manager->ecs, deltatime);
}

void SceneManagerOnRender(SceneManager* manager)
{
	BackgroundRender(&manager->background, CameraGetViewProjectionPtr(manager->camera));

	EcsRender(&manager->ecs, CameraGetViewProjectionPtr(manager->camera));
}

void SceneManagerOnRenderDebug(SceneManager* manager)
{

}
