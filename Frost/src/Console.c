#include "Console.h"

#include <string.h>
#include <stdio.h>

static int _between(int start, int end, int value)
{
	return value >= start && value <= end;
}

void ConsoleInit(Console* console)
{
	memset(console->cmd_buffer, '\0', CONSOLE_MAX_CMD_LENGTH);
	console->cusor_pos = 0;
	console->focus = 1;
}

void ConsoleFocus(Console* console)
{
	console->focus = !console->focus;
}

void ConsoleOnEvent(Console* console, Event* e)
{
	if (!console->focus)
		return;

	if (e->type == EVENT_KEY_TYPED)
	{
		ConsoleCharTyped(console, (char)e->key.keycode);
		e->handled = 1;
	}

	switch (EventKeyPressed(e))
	{
	case KEY_BACKSPACE:
		ConsoleCharRemoveLast(console);
		e->handled = 1;
		break;
	case KEY_ENTER:
		ConsoleExecuteCmd(console);
		e->handled = 1;
		break;
	}

	/* block key events */
	int key = EventKeyPressed(e);
	if (_between(KEY_SPACE, KEY_WORLD_2, key))
		e->handled = 1;
	key = EventKeyReleased(e);
	if (_between(KEY_SPACE, KEY_WORLD_2, key))
		e->handled = 1;
}

typedef enum
{
	CONSOLE_CMD_NONE = -1,
	CONSOLE_CMD_CHANGE,
	CONSOLE_CMD_CREATE
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

static char* _cmd_get_arg(char* buffer, int offset)
{
	char* tok;

	/* get the first token */
	tok = strtok(buffer + offset, " ,.-");

	/* walk through other tokens */
	while (tok != NULL) 
	{
		printf(" %s\n", tok);

		tok = strtok(NULL, " ,.-");
	}

	return tok;
}

static console_cmd _ConsoleGetCmd(Console* console)
{
	if (console->cusor_pos <= 0)
		return CONSOLE_CMD_NONE;

	switch (console->cmd_buffer[0])
	{
	case 'c':
		if (console->cusor_pos > 1)
			switch (console->cmd_buffer[1])
			{
			case 'h': return _cmd_check_keyword(console->cmd_buffer, 2, 4, "ange", CONSOLE_CMD_CHANGE);
			case 'r': return _cmd_check_keyword(console->cmd_buffer, 2, 4, "eate", CONSOLE_CMD_CREATE);
			}
		break;
	}

	return CONSOLE_CMD_NONE;
}

void ConsoleExecuteCmd(Console* console)
{
	/* NULL-terminate cmd */
	ConsoleCharTyped(console, '\0');

	printf("CMD:\n");

	console_cmd cmd = _ConsoleGetCmd(console);
	switch (cmd)
	{
	case CONSOLE_CMD_CHANGE:
		printf(" > change\n");
		_cmd_get_arg(console->cmd_buffer, 6);
		break;
	case CONSOLE_CMD_CREATE:
		printf(" > create\n");
		break;
	default:
		printf(" > unkown\n");
		break;
	}

	/* reset cmd-buffer */
	console->cusor_pos = 0;
}

void ConsoleCharTyped(Console* console, char c)
{
	if (console->cusor_pos >= CONSOLE_MAX_CMD_LENGTH)
		return;

	console->cmd_buffer[console->cusor_pos++] = c;
}

void ConsoleCharRemoveLast(Console* console)
{
	--console->cusor_pos;

	if (console->cusor_pos < 0)
		console->cusor_pos = 0;
}
