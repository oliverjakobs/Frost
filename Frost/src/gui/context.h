#ifndef GUI_CONTEXT_H
#define GUI_CONTEXT_H

#include "clib/clib.h"
#include "clib/dynamic_array.h"

#define GUI_INITIAL_WINDOWS		4
#define GUI_INITIAL_ROWS		8

typedef enum
{
    GUI_HALIGN_NONE,
    GUI_HALIGN_LEFT,
    GUI_HALIGN_CENTER,
    GUI_HALIGN_RIGHT
} gui_halign;

typedef enum
{
    GUI_VALIGN_NONE,
    GUI_VALIGN_TOP,
    GUI_VALIGN_CENTER,
    GUI_VALIGN_BOTTOM
} gui_valign;

typedef enum
{
    GUI_BG_NONE,
    GUI_BG_FILL,
} gui_bg_style;

typedef struct
{
    float x;
    float y;
    float w;
    float h;

    gui_valign v_align;
    gui_halign h_align;
    gui_bg_style bg_style;

    float padding;

    clib_dynamic_array rows;
    float row_y;
} gui_window;

typedef struct
{
    IgnisFont* font;
    IgnisColorRGBA font_color;
    IgnisColorRGBA bg_color;
    IgnisColorRGBA border_color;

    float border_width;
    float separator_line_width;
    float separator_height;
} gui_theme;

typedef struct
{
    clib_dynamic_array windows;

    float width;
    float height;

    gui_theme theme;
} gui_context;

#endif /* !GUI_CONTEXT_H */

