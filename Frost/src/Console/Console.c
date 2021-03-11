#include "Console.h"

#include "Graphics/FontRenderer.h"
#include "Graphics/Primitives2D.h"

#include "toolbox/toolbox.h"
#include "Application/Input.h"

const float CONSOLE_CURSOR_ON = 1.0f;
const float CONSOLE_CURSOR_CYCLE = 1.2f;

const char* CONSOLE_PROMPT = "> ";

void ConsoleInit(Console* console, IgnisFont* font)
{
	memset(console->cmd_buffer, '\0', CONSOLE_MAX_CMD_LENGTH);
	console->cusor_pos = 0;

	console->focus = 0;

	console->cursor_size = 2.0f;
	console->cursor_tick = 0.0f;

	console->font = font;
	console->font_color = IGNIS_WHITE;
	console->bg_color = (IgnisColorRGBA){ 0.1f, 0.1f, 0.1f, 0.8f };
}

void ConsoleOnEvent(Console* console, Event* e)
{
	if (!console->focus) return;

	if (EventCheckType(e, EVENT_KEY_TYPED))
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
	int32_t key = EventKey(e);
	if (tb_between_i32(KEY_SPACE, KEY_WORLD_2, key))
		e->handled = 1;
}

void ConsoleOnUpdate(Console* console, float deltatime)
{
	if (!console->focus) return;

	console->cursor_tick += deltatime;

	if (console->cursor_tick > CONSOLE_CURSOR_CYCLE)
		console->cursor_tick = 0.0f;
}

void ConsoleToggleFocus(Console* console)
{
	console->focus = !console->focus;

	if (!console->focus) ConsoleResetCursor(console);
}

void ConsoleResetCursor(Console* console)
{
	console->cusor_pos = 0;
}

void ConsoleExecuteCmd(Console* console)
{
	/* NULL-terminate cmd */
	ConsoleCharTyped(console, '\0');

	EventHandlerThrowConsoleEvent(EVENT_CONSOLE_EXEC, console->cmd_buffer);

	/* reset cmd-buffer */
	ConsoleResetCursor(console);
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

void ConsoleRenderBackground(const Console* console, float x, float y, float w, float h, const float* proj)
{
	if (!console->focus) return;

	Primitives2DStart(proj);
	Primitives2DFillRect(x, y, w, -h, console->bg_color);
	Primitives2DFlush();
}

void ConsoleRender(const Console* console, float x, float y, float padding, const float* proj)
{
	if (!console->focus || !console->font) return;

	float text_x = x + padding;
	float text_y = y - padding - ignisFontGetHeight(console->font);

	char cursor = (console->cursor_tick <= CONSOLE_CURSOR_ON) ? '_' : '\0';

	FontRendererBindFontColor(console->font, console->font_color);
	FontRendererStart(proj);
	FontRendererRenderTextFormat(text_x, text_y, "%s%.*s%c", CONSOLE_PROMPT, console->cusor_pos, console->cmd_buffer, cursor);
	FontRendererFlush();
}

void ConsoleOut(const Console* console, const char* fmt, ...)
{
	printf("[OUT] ");

	va_list arg;
	va_start(arg, fmt);
	vfprintf(stdout, fmt, arg);
	va_end(arg);

	printf("\n");
}
