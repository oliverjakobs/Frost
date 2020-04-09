#ifndef GUI_ROW_H
#define GUI_ROW_H

#include "Graphics/Renderer.h"

#include "context.h"

/* row content */
typedef struct
{
    char* text;
} gui_row_text;

typedef struct
{
    char* label;
    int state;
} gui_row_button;

typedef enum
{
    GUI_ROW_TEXT,
    GUI_ROW_BUTTON,
    GUI_ROW_CHECKBOX,
    GUI_ROW_RADIOBUTTON,
    GUI_ROW_SEPARATOR
} gui_row_type;

typedef struct gui_row
{
    gui_row_type type;
    union
    {
        gui_row_text text;
        gui_row_button button;
    };
    float x;
    float y;
    float w;
    float h;
} gui_row;

gui_row* gui_row_create_text(char* text, float x, float y, float w, float h);
gui_row* gui_row_create_separator(float x, float y, float w, float h);
gui_row* gui_row_create_button(char* text, float x, float y, float w, float h);

void gui_row_free(gui_row* row);

void gui_row_render(const gui_window* window, gui_row* row, gui_theme theme);
void gui_row_render_font(const gui_window* window, gui_row* row, gui_theme theme);


#endif /* !GUI_ROW_H */
