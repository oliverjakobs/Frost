#include "SceneManager.h"

#include "json/tb_json.h"
#include "json/tb_jwrite.h"

#include "Application/Debugger.h"
#include "Application/Application.h"

#include "Console/Command.h"

#include "SceneLoader.h"

#include "ECS/Systems.h"

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

	EcsInit(&manager->ecs, 2, 2);
	EcsAddUpdateSystem(&manager->ecs, EcsSystemPlayer);
	EcsAddUpdateSystem(&manager->ecs, EcsSystemAnimation);
	EcsAddRenderSystem(&manager->ecs, EcsSystemRender, EcsSystemRenderPre, EcsSystemRenderPost);
	EcsAddRenderSystem(&manager->ecs, EcsSystemDebugRender, EcsSystemDebugRenderPre, EcsSystemDebugRenderPost);

	SceneLoaderLoadRegister(manager, reg);

	manager->console_focus = 0;
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

	EcsDestroy(&manager->ecs);
}

void SceneManagerFocusConsole(SceneManager* manager)
{
	manager->console_focus = !manager->console_focus;

	if (!manager->console_focus) ConsoleResetCursor(&manager->console);
}

void SceneManagerToggleEditmode(SceneManager* manager)
{
	manager->editmode = !manager->editmode;
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

void SceneManagerExecuteCommand(SceneManager* manager, char* cmd_buffer)
{
	console_cmd cmd = cmd_get_type(cmd_buffer);

	switch (cmd)
	{
	case CONSOLE_CMD_CHANGE:
	{
		char* args[1];
		char* spec = cmd_get_args(cmd_buffer, 6, args, 1);

		if (!spec) break;

		if (strcmp(spec, "scene") == 0)
		{
			SceneManagerChangeScene(manager, args[0]);
			ConsoleOut(&manager->console, "Changed Scene to %s", args[0]);
		}
		break;
	}
	case CONSOLE_CMD_CREATE:
	{
		char* args[2];
		char* spec = cmd_get_args(cmd_buffer, 6, args, 2);

		if (!spec) break;

		if (strcmp(spec, "entity") == 0)
		{
			if (!manager->editmode)
			{
				ConsoleOut(&manager->console, "Editmode needs to be active to create an entity.");
				break;
			}

			vec2 pos = CameraGetMousePos(manager->camera, InputMousePositionVec2());
			EcsEntity* entity = SceneLoaderLoadTemplate(args[0], &manager->templates, manager->resources);
			SceneAddEntityPos(manager->scene, entity, atoi(args[1]), pos);
		}
		break;
	}
	case CONSOLE_CMD_LIST:
	{
		char* spec = cmd_get_args(cmd_buffer, 4, NULL, 0);

		if (!spec) break;

		ConsoleOut(&manager->console, "[Console] %s", cmd_buffer);

		if (strcmp(spec, "scenes") == 0)
		{
			CLIB_STRMAP_ITERATE_FOR(&manager->scenes)
			{
				char* name = clib_strmap_iter_get_key(iter);

				ConsoleOut(&manager->console, " - %s %s", name, (strcmp(name, manager->scene_name) == 0) ? "(active)" : "");
			}
		}
		else if (strcmp(spec, "entities") == 0)
		{
			for (int i = 0; i < manager->scene->entities.size; ++i)
			{
				EcsEntity* e = entities_dynamic_array_get(&manager->scene->entities, i);

				ConsoleOut(&manager->console, " - %s (%d)", e->name, e->z_index);
			}
		}
		else if (strcmp(spec, "templates") == 0)
		{
			CLIB_STRMAP_ITERATE_FOR(&manager->templates)
			{
				char* name = clib_strmap_iter_get_key(iter);
				char* templ = clib_strmap_iter_get_value(iter);

				ConsoleOut(&manager->console, " - %s: %s", name, templ);
			}
		}
		else if (strcmp(spec, "res") == 0)
		{
			ConsoleOut(&manager->console, "Textures:");
			CLIB_DICT_ITERATE_FOR(&manager->resources->textures)
			{
				ConsoleOut(&manager->console, " - %s", clib_dict_iter_get_key(iter));
			}

			ConsoleOut(&manager->console, "Fonts:");
			CLIB_DICT_ITERATE_FOR(&manager->resources->fonts)
			{
				ConsoleOut(&manager->console, " - %s", clib_dict_iter_get_key(iter));
			}
		}
		break;
	}
	case CONSOLE_CMD_REMOVE:
		ConsoleOut(&manager->console, " > remove");
		break;
	case CONSOLE_CMD_SAVE:
	{
		char* spec = cmd_get_args(cmd_buffer, 4, NULL, 0);

		if (!spec) break;

		if (strcmp(spec, "scene") == 0)
		{
			char* path = clib_strmap_get(&manager->scenes, manager->scene_name);
			if (!path)
			{
				ConsoleOut(&manager->console, "Couldn't find path for %s", manager->scene_name);
				break;
			}

			SceneLoaderSaveScene(manager->scene, path, manager->resources);
			ConsoleOut(&manager->console, "Saved scene (%s) to %s", manager->scene_name, path);
		}

		break;
	}
	default:
		ConsoleOut(&manager->console, "Unkown command");
		break;
	}

	SceneManagerFocusConsole(manager);
}

void SceneManagerOnEvent(SceneManager* manager, Event e)
{
	if (e.type == EVENT_CONSOLE_EXEC)
	{
		SceneManagerExecuteCommand(manager, e.console.cmd);
	}

	if (manager->console_focus)
		ConsoleOnEvent(&manager->console, &e);

	switch (EventKeyPressed(&e))
	{
	case KEY_F1:
		SceneManagerToggleEditmode(manager);
		break;
	case KEY_F2:
		SceneManagerFocusConsole(manager);
		break;
	}

	if (manager->editmode)
		SceneEditorOnEvent(&manager->editor, manager->scene, e);

	SceneOnEvent(manager->scene, e);
}

void SceneManagerOnUpdate(SceneManager* manager, float deltatime)
{
	if (manager->console_focus)
	{
		ConsoleOnUpdate(&manager->console, deltatime);
	}
	else if (manager->editmode)
	{
		SceneEditorOnUpdate(&manager->editor, manager->scene, deltatime);
	}
	else
	{
		SceneOnUpdate(manager->scene, &manager->ecs, deltatime);
	}
}

void SceneManagerOnRender(SceneManager* manager)
{
	SceneOnRender(manager->scene, &manager->ecs);

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
