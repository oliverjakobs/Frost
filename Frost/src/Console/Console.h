#ifndef CONSOLE_H
#define CONSOLE_H

#define CONSOLE_MAX_CMD_LENGTH	64

#define CONSOLE_OUT_BUF_SIZE	64
#define CONSOLE_OUT_ROW_SIZE	64

#include "Ignis/Ignis.h"
#include "Minimal/Minimal.h"

typedef struct
{
	int focus;

	char cmd_buffer[CONSOLE_MAX_CMD_LENGTH];
	int cusor_pos;

	float cursor_size;
	float cursor_tick;

	void* user_data;
	void (*execute)(void*, const char*);

	IgnisFont* font;
	IgnisColorRGBA font_color;
	IgnisColorRGBA bg_color;
} Console;

void ConsoleInit(Console* console, void* user_data, void (*execute)(void*, const char*), IgnisFont* font);

MinimalBool ConsoleOnEvent(Console* console, const MinimalEvent* e);
void ConsoleOnUpdate(Console* console, float deltatime);

void ConsoleToggleFocus(Console* console);
void ConsoleResetCursor(Console* console);

void ConsoleExecuteCmd(Console* console);

void ConsoleCharTyped(Console* console, char c);
void ConsoleCharRemoveLast(Console* console);

void ConsoleRenderBackground(const Console* console, float x, float y, float w, float h, const float* proj);
void ConsoleRender(const Console* console, float x, float y, float padding, const float* proj);

void ConsoleOut(const Console* console, const char* fmt, ...);

#endif /* !CONSOLE_H */
