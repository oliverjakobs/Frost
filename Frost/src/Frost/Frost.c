#include "Frost.h"

#include "Console/Command.h"

typedef enum
{
	CONSOLE_CMD_NONE = -1,
	CONSOLE_CMD_CHANGE,
	CONSOLE_CMD_CREATE,
	CONSOLE_CMD_LIST,
	CONSOLE_CMD_REMOVE,
	CONSOLE_CMD_SAVE
} ConsoleCmd;

static ConsoleCmd _CmdGetType(const char* buffer)
{
	ConsoleCmd cmd = CONSOLE_CMD_NONE;
	size_t cmd_length = strlen(buffer);

	switch (buffer[0])
	{
	case 'c':
		if (cmd_length > 1)
			switch (buffer[1])
			{
			case 'h': cmd = cmd_check_keyword(buffer, "change", CONSOLE_CMD_CHANGE); break;
			case 'r': cmd = cmd_check_keyword(buffer, "create", CONSOLE_CMD_CREATE); break;
			}
		break;
	case 'l': cmd = cmd_check_keyword(buffer, "list", CONSOLE_CMD_LIST); break;
	case 'r': cmd = cmd_check_keyword(buffer, "remove", CONSOLE_CMD_REMOVE); break;
	case 's': cmd = cmd_check_keyword(buffer, "save", CONSOLE_CMD_SAVE); break;
	}

	return cmd;
}

void FrostExecuteConsoleCommand(Console* console, Scene* scene, SceneEditor* editor, const char* cmd_buffer)
{
	ConsoleCmd cmd = _CmdGetType(cmd_buffer);

	switch (cmd)
	{
	case CONSOLE_CMD_CHANGE:
	{
		char* args[1];
		char* spec = cmd_get_args(cmd_buffer, 6, args, 1);

		if (!spec) break;

		if (strcmp(spec, "scene") == 0)
		{
			SceneChangeActive(scene, args[0]);
			SceneEditorReset(editor);
			ConsoleOut(console, "Changed Scene to %s", args[0]);
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
			if (!editor->active)
			{
				ConsoleOut(console, "Editmode needs to be active to create an entity.");
				break;
			}

			vec2 pos = CameraGetMousePosView(scene->camera, InputMousePositionVec2());

			if (SceneLoadTemplate(scene, args[0], EntityGetNextID(), pos, atoi(args[1])))
				ConsoleOut(console, "Created entity with template %s", args[0]);
		}
		break;
	}
	case CONSOLE_CMD_LIST:
	{
		char* spec = cmd_get_args(cmd_buffer, 4, NULL, 0);

		if (!spec) break;

		ConsoleOut(console, "%s", cmd_buffer);

		if (strcmp(spec, "scenes") == 0)
		{
			CLIB_STRMAP_ITERATE_FOR(&scene->scene_register)
			{
				const char* name = clib_strmap_iter_get_key(iter);

				ConsoleOut(console, " - %s %s", name, (strcmp(name, scene->name) == 0) ? "(active)" : "");
			}
		}
		else if (strcmp(spec, "entities") == 0)
		{
			for (size_t i = 0; i < EcsGetComponentList(&scene->ecs, COMPONENT_TEMPLATE)->list.used; ++i)
			{
				Template* templ = EcsGetOrderComponent(&scene->ecs, i, COMPONENT_TEMPLATE);

				if (templ)
					ConsoleOut(console, " - %d \t | %s", *(EntityID*)templ, templ->templ);
			}
		}
		else if (strcmp(spec, "templates") == 0)
		{
			CLIB_STRMAP_ITERATE_FOR(&scene->templates)
			{
				const char* name = clib_strmap_iter_get_key(iter);
				char* templ = clib_strmap_iter_get_value(iter);

				ConsoleOut(console, " - %s: %s", name, templ);
			}
		}
		else if (strcmp(spec, "res") == 0)
		{
			ConsoleOut(console, "Textures:");
			CLIB_DICT_ITERATE_FOR(&scene->resources->textures, iter)
			{
				ConsoleOut(console, " - %s", clib_dict_iter_get_key(iter));
			}

			ConsoleOut(console, "Fonts:");
			CLIB_DICT_ITERATE_FOR(&scene->resources->fonts, iter)
			{
				ConsoleOut(console, " - %s", clib_dict_iter_get_key(iter));
			}
		}
		break;
	}
	case CONSOLE_CMD_REMOVE:
		ConsoleOut(console, " > remove");
		break;
	case CONSOLE_CMD_SAVE:
	{
		char* spec = cmd_get_args(cmd_buffer, 4, NULL, 0);

		if (!spec) break;

		if (strcmp(spec, "scene") == 0)
		{
			char* path = clib_strmap_find(&scene->scene_register, scene->name);
			if (!path)
			{
				ConsoleOut(console, "Couldn't find path for %s", scene->name);
				break;
			}

			SceneSave(scene, path);
			ConsoleOut(console, "Saved scene (%s) to %s", scene->name, path);
		}
		break;
	}
	default:
		ConsoleOut(console, "Unkown command");
		break;
	}

	ConsoleToggleFocus(console);
}
