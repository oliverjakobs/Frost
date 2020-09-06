#include "Command.h"

#include <string.h>
#include <stdio.h>

uint32_t cmd_check_keyword(const char* buffer, const char* rest, uint32_t cmd)
{
	size_t length = strlen(rest);

	if (buffer[length] != ' ' && buffer[length] != '\0')
		return 0;

	if (memcmp(buffer, rest, length) == 0)
		return cmd;

	return 0;
}

char* cmd_get_args(const char* buffer, int offset, char** args, int arg_count)
{
	const char* delimiter = " ";

	/* get the first token */
	char* spec = strtok((char*)buffer + offset, delimiter);

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
