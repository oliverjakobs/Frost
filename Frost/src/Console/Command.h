#ifndef COMMAND_H
#define COMMAND_H

typedef enum
{
	CONSOLE_CMD_NONE = -1,
	CONSOLE_CMD_CHANGE,
	CONSOLE_CMD_CREATE,
	CONSOLE_CMD_LIST,
	CONSOLE_CMD_REMOVE,
	CONSOLE_CMD_SAVE
} console_cmd;


char* cmd_get_args(char* buffer, int offset, char** args, int arg_count);

console_cmd cmd_get_type(char* buffer);

#endif /* COMMAND_H */
