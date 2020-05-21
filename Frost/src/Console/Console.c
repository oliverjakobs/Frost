#include "Console.h"

#include "clib/clib.h"

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
	if (BETWEEN(KEY_SPACE, KEY_WORLD_2, key))
		e->handled = 1;
	key = EventKeyReleased(e);
	if (BETWEEN(KEY_SPACE, KEY_WORLD_2, key))
		e->handled = 1;
}

void ConsoleExecuteCmd(Console* console)
{
	/* NULL-terminate cmd */
	ConsoleCharTyped(console, '\0');

	EventHandlerThrowConsoleEvent(EVENT_CONSOLE_EXEC, console->cmd_buffer);

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

void ConsoleRender(Console* console, float x, float y, float w, float h, float padding, const float* proj)
{
	Primitives2DStart(proj);

	Primitives2DFillRect(x, y, w, -h, (IgnisColorRGBA){ 0.1f, 0.1f, 0.1f, 0.8f });

	Primitives2DFlush();

	FontRendererStart(proj);

	FontRendererRenderTextFormat(x + padding, y - padding, "> %.*s", console->cusor_pos, console->cmd_buffer);

	FontRendererFlush();
}
