#include "Console.h"

#include "Graphics/FontRenderer.h"
#include "Graphics/Primitives2D.h"

#include "clib/clib.h"
#include "Application/Input.h"

const float CONSOLE_CURSOR_ON = 1.0f;
const float CONSOLE_CURSOR_CYCLE = 1.2f;

void ConsoleInit(Console* console, IgnisFont* font)
{
	memset(console->cmd_buffer, '\0', CONSOLE_MAX_CMD_LENGTH);
	console->cusor_pos = 0;

	console->cursor_size = 2.0f;
	console->cursor_tick = 0.0f;

	console->font = font;
	console->font_color = IGNIS_WHITE;
	console->bg_color = (IgnisColorRGBA){ 0.1f, 0.1f, 0.1f, 0.8f };

	console->prompt = "> ";

	console->focus = 1;
	console->show_cursor = 1;
}

void ConsoleFocus(Console* console)
{
	console->focus = !console->focus;

	if (!console->focus) console->cusor_pos = 0;
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
	int key = EventKey(e);
	if (BETWEEN(KEY_SPACE, KEY_WORLD_2, key))
		e->handled = 1;
}

void ConsoleOnUpdate(Console* console, float deltatime)
{
	console->cursor_tick += deltatime;

	console->show_cursor = (console->cursor_tick <= CONSOLE_CURSOR_ON);

	if (console->cursor_tick > CONSOLE_CURSOR_CYCLE)
		console->cursor_tick = 0.0f;
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
	float text_x = x + padding;
	float text_y = y - padding;
	float cursor_x = text_x 
		+ ignisFontGetTextWidth(console->font, console->cmd_buffer, console->cusor_pos) 
		+ ignisFontGetTextWidth(console->font, console->prompt, strlen(console->prompt));
	float cursor_y = y - (padding / 2.0f);
	float cursor_w = console->cursor_size;
	float cursor_h = -(h - (padding * 1.5f));

	Primitives2DStart(proj);

	Primitives2DFillRect(x, y, w, -h, console->bg_color);

	if (console->show_cursor)
		Primitives2DFillRect(cursor_x, cursor_y, cursor_w, cursor_h, console->font_color);

	Primitives2DFlush();

	if (console->font)
		FontRendererBindFontColor(console->font, console->font_color);

	FontRendererStart(proj);

	FontRendererRenderTextFormat(text_x, text_y, "%s%.*s", console->prompt, console->cusor_pos, console->cmd_buffer);

	FontRendererFlush();
}

void ConsoleOut(Console* console, const char* fmt, ...)
{
	char row[CONSOLE_OUT_ROW_SIZE];
	memset(row, '\0', CONSOLE_OUT_ROW_SIZE);

	va_list ap;
	va_start(ap, fmt);
	vsprintf(row, fmt, ap);
	va_end(ap);

	printf("[OUT] %s\n", row);
}
