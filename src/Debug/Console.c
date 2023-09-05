#include "Console.h"

#include "ignis/renderer/renderer.h"

#include "Minimal/Input.h"

const float CONSOLE_CURSOR_ON = 1.0f;
const float CONSOLE_CURSOR_CYCLE = 1.2f;

const char* CONSOLE_PROMPT = "> ";

void ConsoleInit(Console* console, void* user_data, ConsoleCB execute, IgnisFont* font)
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

int ConsoleOnEvent(Console* console, const MinimalEvent* e)
{
    if (!console->focus) return 0;

    switch (minimalEventKeyPressed(e))
    {
    case MINIMAL_KEY_BACKSPACE: ConsoleCharRemoveLast(console); return 1;
    case MINIMAL_KEY_ENTER:     ConsoleExecuteCmd(console);     return 1;
    }

    /* TODO: fix backspace char event */
    if (minimalCheckEventType(e, MINIMAL_EVENT_CHAR))
    {
        ConsoleCharTyped(console, minimalEventChar(e));
        return 1;
    }

    /* block key events */
    int32_t key = minimalEventKey(e);
    return key != MINIMAL_KEY_UNKNOWN;
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

    console->execute(console, console->user_data, console->cmd_buffer);

    /* reset cmd-buffer */
    ConsoleResetCursor(console);
}

void ConsoleCharTyped(Console* console, char c)
{
    if (console->cusor_pos >= CONSOLE_MAX_CMD_LENGTH) return;

    console->cmd_buffer[console->cusor_pos++] = c;
}

void ConsoleCharRemoveLast(Console* console)
{
    if (console->cusor_pos > 0) console->cusor_pos--;
}

void ConsoleRender(const Console* console, float x, float y, float w, float h, float padding, const float* proj)
{
    if (!console->focus || !console->font) return;

    ignisPrimitivesRendererSetViewProjection(proj);
    ignisPrimitives2DFillRect(x, y, w, -h, console->bg_color);
    ignisPrimitivesRendererFlush();

    float text_x = x + padding;
    float text_y = y - padding - ignisFontGetHeight(console->font);

    char cursor = (console->cursor_tick <= CONSOLE_CURSOR_ON) ? '_' : '\0';

    ignisFontRendererBindFontColor(console->font, console->font_color);
    ignisFontRendererSetProjection(proj);
    ignisFontRendererRenderTextFormat(text_x, text_y, "%s%.*s%c", CONSOLE_PROMPT, console->cusor_pos, console->cmd_buffer, cursor);
    ignisFontRendererFlush();
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

uint32_t ConsoleCheckCMDKeyword(const char* buffer, const char* rest, uint32_t cmd)
{
    size_t length = strlen(rest);

    if (buffer[length] != ' ' && buffer[length] != '\0') return 0;
    if (memcmp(buffer, rest, length) == 0)               return cmd;

    return 0;
}

char* ConsoleGetCMDArgs(const char* buffer, int offset, char** args, int arg_count)
{
    const char* delimiter = " ";

    /* get the first token */
    char* spec = strtok((char*)buffer + offset, delimiter);

    /* walk through other tokens */
    for (int i = 0; i < arg_count; ++i)
    {
        args[i] = strtok(NULL, delimiter);

        if (!args[i])
        {
            printf("Too few args\n");
            return NULL;
        }
    }

    if (strtok(NULL, delimiter))
    {
        printf("Too many args\n");
        return NULL;
    }

    return spec;
}