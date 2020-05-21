#include "Command.h"

#include <string.h>
#include <stdio.h>

typedef enum
{
	CONSOLE_CMD_NONE = -1,
	CONSOLE_CMD_CHANGE,
	CONSOLE_CMD_CREATE,
	CONSOLE_CMD_LIST,
	CONSOLE_CMD_REGISTER,
	CONSOLE_CMD_REMOVE,
	CONSOLE_CMD_SAVE
} ConsoleCmd;

/* Check the rest of a potential cmd’s lexeme */
static ConsoleCmd _CommandCheckKeyword(char* buffer, int s, int l, const char* r, ConsoleCmd cmd)
{
	if (buffer[s + l] != ' ' && buffer[s + l] != '\0')
		return CONSOLE_CMD_NONE;

	if (memcmp(buffer + s, r, l) == 0)
		return cmd;

	return CONSOLE_CMD_NONE;
}

static char* _CommandGetArgs(char* buffer, int offset, char** args, int arg_count)
{
	char* delimiter = " ";

	/* get the first token */
	char* spec = strtok(buffer + offset, delimiter);

	/* walk through other tokens */
	for (int i = 0; i < arg_count; ++i)
	{
		args[i] = strtok(NULL, delimiter);

		if (!args[i])
		{
			printf("Too few args\n");
			return NULL;
		}
	}

	if (strtok(NULL, delimiter))
	{
		printf("Too many args\n");
		return NULL;
	}

	return spec;
}

static ConsoleCmd _CommandGetType(char* buffer)
{
	int length = strlen(buffer);

	switch (buffer[0])
	{
	case 'c':
		if (length > 1)
			switch (buffer[1])
			{
			case 'h': return _CommandCheckKeyword(buffer, 2, 4, "ange", CONSOLE_CMD_CHANGE);
			case 'r': return _CommandCheckKeyword(buffer, 2, 4, "eate", CONSOLE_CMD_CREATE);
			}
		break;
	case 'l':
		return _CommandCheckKeyword(buffer, 1, 3, "ist", CONSOLE_CMD_LIST);
	case 'r':
		if (length > 2 && buffer[1] == 'e')
			switch (buffer[2])
			{
			case 'g': return _CommandCheckKeyword(buffer, 3, 5, "ister", CONSOLE_CMD_REGISTER);
			case 'm': return _CommandCheckKeyword(buffer, 3, 3, "ove", CONSOLE_CMD_REMOVE);
			}
		break;
	case 's':
		return _CommandCheckKeyword(buffer, 1, 3, "ave", CONSOLE_CMD_SAVE);
	}

	return CONSOLE_CMD_NONE;
}

void CommandExecute(SceneManager* manager, char* cmd_buffer)
{
	ConsoleCmd cmd = _CommandGetType(cmd_buffer);

	switch (cmd)
	{
	case CONSOLE_CMD_CHANGE:
	{
		char* args[1];
		char* spec = _CommandGetArgs(cmd_buffer, 6, args, 1);

		if (!spec) break;

		if (strcmp(spec, "scene") == 0)
			SceneManagerChangeScene(manager, args[0]);
		else if (strcmp(spec, "layer") == 0)
		{
			int layer;

			if (strcmp(args[0], "up") == 0)
				layer = manager->editor.layer + 1;
			else if (strcmp(args[0], "down") == 0)
				layer = manager->editor.layer - 1;
			else
				layer = atoi(args[0]);

			SceneEditorChangeLayer(&manager->editor, layer, manager->scene->max_layer);
		}
		break;
	}
	case CONSOLE_CMD_CREATE:
	{
		char* args[1];
		char* spec = _CommandGetArgs(cmd_buffer, 6, args, 1);

		if (!spec) break;

		if (strcmp(spec, "entity") == 0)
		{
			if (!manager->editmode)
			{
				printf("Editmode needs to be active to create an entity.\n");
				break;
			}

			vec2 pos = CameraGetMousePos(manager->camera, InputMousePositionVec2());

			char* templ = SceneManagerGetTemplate(manager, args[0]);

			if (templ)
				SceneAddEntityPos(manager->scene, EcsEntityLoadTemplate(templ, manager->resources), manager->editor.layer, pos);
		}
		break;
	}
	case CONSOLE_CMD_LIST:
	{
		char* spec = _CommandGetArgs(cmd_buffer, 4, NULL, 0);

		if (!spec) break;

		printf("[Console] %s\n", cmd_buffer);

		if (strcmp(spec, "scenes") == 0)
		{
			struct clib_hashmap_iter* iter = clib_hashmap_iter(&manager->scenes);

			while (iter)
			{
				char* name = (char*)clib_hashmap_iter_get_key(iter);

				printf(" - %s %s\n", name, (strcmp(name, manager->scene_name) == 0) ? "(active)" : "");

				iter = clib_hashmap_iter_next(&manager->scenes, iter);
			}
		}
		else if (strcmp(spec, "entities") == 0)
		{
			for (int l = 0; l < manager->scene->max_layer; ++l)
			{
				for (int i = 0; i < manager->scene->layers[l].size; ++i)
				{
					EcsEntity* e = layer_vector_get(&manager->scene->layers[l], i);

					printf(" - %s (%d)\n", e->name, l);
				}
			}
		}
		break;
	}
	case CONSOLE_CMD_REGISTER:
		printf(" > register\n");
		break;
	case CONSOLE_CMD_REMOVE:
		printf(" > remove\n");
		break;
	case CONSOLE_CMD_SAVE:
	{
		char* spec = _CommandGetArgs(cmd_buffer, 4, NULL, 0);

		if (!spec) break;

		if (strcmp(spec, "scene") == 0)
		{
			char* path = clib_strmap_get(&manager->scenes, manager->scene_name);
			if (!path)
			{
				printf("Couldn't find path for %s\n", manager->scene_name);
				break;
			}

			SceneManagerSaveScene(manager, manager->scene, path);
		}

		break;
	}
	default:
		printf(" > unkown\n");
		break;
	}
}
