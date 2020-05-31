#include "SceneManager.h"

#include "json/tb_json.h"
#include "json/tb_jwrite.h"

#include "Application/Debugger.h"
#include "Application/Application.h"

#include "Console/Command.h"

#include "SceneLoader.h"

int SceneManagerInit(SceneManager* manager, const char* reg, ResourceManager* resources, Camera* camera, float gridsize, uint16_t padding)
{
	manager->resources = resources;
	manager->camera = camera;

	manager->editmode = 1;
	SceneEditorInit(&manager->editor, 400.0f, gridsize, gridsize * padding);

	manager->scene = (Scene*)malloc(sizeof(Scene));
	if (!manager->scene)
	{
		DEBUG_ERROR("[SceneManager] Failed to allocate memory for scene name\n");
		return 0;
	}
	memset(manager->scene_name, '\0', APPLICATION_STR_LEN);

	clib_strmap_init(&manager->scenes, 0);
	clib_strmap_init(&manager->templates, 0);

	SceneLoaderLoadRegister(manager, reg);

	ConsoleInit(&manager->console, ResourceManagerGetFont(manager->resources, "gui"));

	return 1;
}

void SceneManagerDestroy(SceneManager* manager)
{
	if (manager->scene_name[0] != '\0')
		SceneQuit(manager->scene);

	free(manager->scene);
	clib_strmap_destroy(&manager->scenes);
	clib_strmap_destroy(&manager->templates);
}

void SceneManagerChangeScene(SceneManager* manager, const char* name)
{
	if (strcmp(manager->scene_name, name) != 0)
	{
		/* Check if scene is in the register */
		char* path = clib_strmap_get(&manager->scenes, name);
		if (!path)
		{
			DEBUG_ERROR("[SceneManager] Couldn't find scene: %s\n", name);
			return;
		}

		// Exit old Scene
		if (manager->scene_name[0] != '\0')
			SceneQuit(manager->scene);

		// Enter new scene
		SceneLoaderLoadScene(manager->scene, path, manager->camera, manager->resources, &manager->templates);
		SceneEditorReset(&manager->editor);
		strcpy(manager->scene_name, name);
	}
}

void SceneManagerOnEvent(SceneManager* manager, Event e)
{
	if (e.type == EVENT_CONSOLE_EXEC)
	{
		CommandExecute(manager, e.console.cmd);
	}

	ConsoleOnEvent(&manager->console, &e);

	if (e.type == EVENT_KEY_PRESSED)
	{
		switch (e.key.keycode)
		{
		case KEY_F1:
			manager->editmode = !manager->editmode;
			break;
		case KEY_F2:
			ConsoleFocus(&manager->console);
			break;
		}
	}

	if (manager->editmode)
		SceneEditorOnEvent(&manager->editor, manager->scene, e);

	SceneOnEvent(manager->scene, e);
}

void SceneManagerOnUpdate(SceneManager* manager, float deltatime)
{
	if (manager->console.focus)
	{
		ConsoleOnUpdate(&manager->console, deltatime);
	}
	else if (manager->editmode)
	{
		SceneEditorOnUpdate(&manager->editor, manager->scene, deltatime);
	}
	else
	{
		SceneOnUpdate(manager->scene, deltatime);
	}
}

void SceneManagerOnRender(SceneManager* manager)
{
	SceneOnRender(manager->scene);

	if (manager->editmode)
	{
		SceneEditorOnRender(&manager->editor, manager->scene);
	}
}

void SceneManagerOnRenderDebug(SceneManager* manager)
{
	if (!manager->editmode)
		SceneOnRenderDebug(manager->scene);
}

void SceneManagerOnRenderGui(SceneManager* manager)
{

}
