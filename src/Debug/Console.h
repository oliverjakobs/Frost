#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

#define CONSOLE_MAX_CMD_LENGTH	64

#define CONSOLE_OUT_BUF_SIZE	64
#define CONSOLE_OUT_ROW_SIZE	64

#include "Ignis/Ignis.h"
#include "Minimal/Minimal.h"

#include <stdint.h>

typedef struct Console Console;
struct Console
{
    int focus;

    char cmd_buffer[CONSOLE_MAX_CMD_LENGTH];
    int cusor_pos;

    float cursor_size;
    float cursor_tick;

    void* user_data;
    void (*execute)(Console*, void*, const char*);

    IgnisFont* font;
    IgnisColorRGBA font_color;
    IgnisColorRGBA bg_color;
};

void ConsoleInit(Console* console, void* user_data, void (*execute)(Console*, void*, const char*), IgnisFont* font);

MinimalBool ConsoleOnEvent(Console* console, const MinimalEvent* e);
void ConsoleOnUpdate(Console* console, float deltatime);

void ConsoleToggleFocus(Console* console);
void ConsoleResetCursor(Console* console);

void ConsoleExecuteCmd(Console* console);

void ConsoleCharTyped(Console* console, char c);
void ConsoleCharRemoveLast(Console* console);

void ConsoleRender(const Console* console, float x, float y, float w, float h, float padding, const float* proj);

void ConsoleOut(const Console* console, const char* fmt, ...);

/* command utility */
/*
 * Check the rest of a potential cmd’s lexeme
 * Returns cmd if it is a match or 0 if not
 */
uint32_t ConsoleCheckCMDKeyword(const char* buffer, const char* rest, uint32_t cmd);
char* ConsoleGetCMDArgs(const char* buffer, int offset, char** args, int arg_count);

#endif /* !DEBUG_CONSOLE_H */
