#ifndef CONSOLE_H
#define CONSOLE_H

#define CONSOLE_MAX_CMD_LENGTH	32

#include "Application/EventHandler.h"

typedef struct
{
	char cmd_buffer[CONSOLE_MAX_CMD_LENGTH];
	int cusor_pos;

	int focus;
} Console;

void ConsoleInit(Console* console);
void ConsoleFocus(Console* console);

void ConsoleOnEvent(Console* console, Event* e);

void ConsoleExecuteCmd(Console* console);

void ConsoleCharTyped(Console* console, char c);
void ConsoleCharRemoveLast(Console* console);

#endif /* !CONSOLE_H */
