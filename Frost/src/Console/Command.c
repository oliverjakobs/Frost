#include "Command.h"

#include <string.h>
#include <stdio.h>

/* Check the rest of a potential cmd’s lexeme */
static console_cmd _cmd_check_keyword(char* buffer, const char* r, console_cmd cmd)
{
	size_t length = strlen(r);

	if (buffer[length] != ' ' && buffer[length] != '\0')
		return CONSOLE_CMD_NONE;

	if (memcmp(buffer, r, length) == 0)
		return cmd;

	return CONSOLE_CMD_NONE;
}

char* cmd_get_args(char* buffer, int offset, char** args, int arg_count)
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

console_cmd cmd_get_type(char* buffer)
{
	size_t length = strlen(buffer);

	switch (buffer[0])
	{
	case 'c':
		if (length > 1)
			switch (buffer[1])
			{
			case 'h': return _cmd_check_keyword(buffer, "change", CONSOLE_CMD_CHANGE);
			case 'r': return _cmd_check_keyword(buffer, "create", CONSOLE_CMD_CREATE);
			}
		break;
	case 'l': return _cmd_check_keyword(buffer, "list", CONSOLE_CMD_LIST);
	case 'r': return _cmd_check_keyword(buffer, "remove", CONSOLE_CMD_REMOVE);
	case 's': return _cmd_check_keyword(buffer, "save", CONSOLE_CMD_SAVE);
	}

	return CONSOLE_CMD_NONE;
}
