#ifndef CONSOLE_H
#define CONSOLE_H

#define CONSOLE_MAX_CMD_LENGTH	64

#define CONSOLE_OUT_BUF_SIZE	8
#define CONSOLE_OUT_ROW_SIZE	64

#include "Application/EventHandler.h"
#include "Application/defines.h"

typedef struct
{
	char cmd_buffer[CONSOLE_MAX_CMD_LENGTH];
	int cusor_pos;

	int focus;
} Console;

void ConsoleInit(Console* console);
void ConsoleFocus(Console* console);

void ConsoleOnEvent(Console* console, Event* e);
void ConsoleOnUpdate(Console* console, float deltatime);

void ConsoleExecuteCmd(Console* console);

void ConsoleCharTyped(Console* console, char c);
void ConsoleCharRemoveLast(Console* console);

void ConsoleRender(Console* console, float x, float y, float w, float h, float padding, const float* proj);

void ConsoleOut(Console* console, const char* fmt, ...);

#endif /* !CONSOLE_H */
