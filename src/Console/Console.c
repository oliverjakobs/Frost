﻿#include "Console.h"

#include "Graphics/FontRenderer.h"
#include "Graphics/Primitives2D.h"

#include "toolbox/toolbox.h"

const float CONSOLE_CURSOR_ON = 1.0f;
const float CONSOLE_CURSOR_CYCLE = 1.2f;

const char* CONSOLE_PROMPT = "> ";

void ConsoleInit(Console* console, void* user_data, void(*execute)(void*, const char*), IgnisFont* font)
{
	memset(console->cmd_buffer, '\0', CONSOLE_MAX_CMD_LENGTH);
	console->cusor_pos = 0;

	console->focus = 0;

	console->cursor_size = 2.0f;
	console->cursor_tick = 0.0f;

	console->user_data = user_data;
	console->execute = execute;

	console->font = font;
	console->font_color = IGNIS_WHITE;
	console->bg_color = (IgnisColorRGBA){ 0.1f, 0.1f, 0.1f, 0.8f };
}

MinimalBool ConsoleOnEvent(Console* console, const MinimalEvent* e)
{
	if (!console->focus) return 0;

	switch (MinimalEventKeyPressed(e))
	{
	case MINIMAL_KEY_BACKSPACE:	ConsoleCharRemoveLast(console); return 1;
	case MINIMAL_KEY_ENTER:		ConsoleExecuteCmd(console);		return 1;
	}

	/* TODO: fix backspace char event */
	if (MinimalCheckEventType(e, MINIMAL_EVENT_CHAR))
	{
		ConsoleCharTyped(console, MinimalEventChar(e));
		return 1;
	}

	/* block key events */
	int32_t key = MinimalEventKey(e);
	return MinimalKeycodeValid(key);
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

	console->execute(console->user_data, console->cmd_buffer);

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
