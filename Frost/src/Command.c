#include "Command.h"

#include <string.h>
#include <stdio.h>

typedef enum
{
	CONSOLE_CMD_NONE = -1,
	CONSOLE_CMD_CHANGE,
	CONSOLE_CMD_CREATE,
	CONSOLE_CMD_REGISTER,
	CONSOLE_CMD_REMOVE,
	CONSOLE_CMD_SHOW
} console_cmd;

/* Check the rest of a potential cmd’s lexeme */
static console_cmd _cmd_check_keyword(char* buffer, int s, int l, const char* r, console_cmd cmd)
{
	if (buffer[s + l] != ' ' && buffer[s + l] != '\0')
		return CONSOLE_CMD_NONE;

	if (memcmp(buffer + s, r, l) == 0)
		return cmd;

	return CONSOLE_CMD_NONE;
}

static char* _cmd_get_args(char* buffer, int offset, char** args, int arg_count)
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

static console_cmd _cmd_get_type(char* buffer)
{
	int length = strlen(buffer);

	switch (buffer[0])
	{
	case 'c':
		if (length > 1)
			switch (buffer[1])
			{
			case 'h': return _cmd_check_keyword(buffer, 2, 4, "ange", CONSOLE_CMD_CHANGE);
			case 'r': return _cmd_check_keyword(buffer, 2, 4, "eate", CONSOLE_CMD_CREATE);
			}
		break;
	case 'r':
		if (length > 2 && buffer[1] == 'e')
			switch (buffer[2])
			{
			case 'g': return _cmd_check_keyword(buffer, 3, 5, "ister", CONSOLE_CMD_REGISTER);
			case 'm': return _cmd_check_keyword(buffer, 3, 3, "ove", CONSOLE_CMD_REMOVE);
			}
		break;
	case 's':
		return _cmd_check_keyword(buffer, 1, 3, "how", CONSOLE_CMD_SHOW);
	}

	return CONSOLE_CMD_NONE;
}

void CommandExecute(Application* app, SceneManager* manager, char* cmd_buffer)
{
	console_cmd cmd = _cmd_get_type(cmd_buffer);

	switch (cmd)
	{
	case CONSOLE_CMD_CHANGE:
	{
		char* args[1];
		char* spec = _cmd_get_args(cmd_buffer, 6, args, 1);

		if (!spec) break;

		if (strcmp(spec, "scene") == 0)
			SceneManagerChangeScene(manager, args[0]);
		else if (strcmp(spec, "layer") == 0)
			SceneEditorChangeLayer(&manager->editor, atoi(args[0]), manager->scene->max_layer);
		break;
	}
	case CONSOLE_CMD_CREATE:
		printf(" > create\n");
		break;
	case CONSOLE_CMD_REGISTER:
		printf(" > register\n");
		break;
	case CONSOLE_CMD_REMOVE:
		printf(" > remove\n");
		break;
	case CONSOLE_CMD_SHOW:
		printf(" > show\n");
		break;
	default:
		printf(" > unkown\n");
		break;
	}
}
