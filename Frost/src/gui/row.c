#include "row.h"

#include <stdlib.h>

gui_row* gui_row_create(gui_row_type type, float x, float y, float w, float h)
{
    gui_row* row = (gui_row*)malloc(sizeof(gui_row));

    if (!row) return NULL;

    row->type = type;
    row->x = x;
    row->y = y;
    row->w = w;
    row->h = h;

    return row;
}

gui_row* gui_row_create_text(char* text, float x, float y, float w, float h)
{
    gui_row* row = gui_row_create(GUI_ROW_TEXT, x, y, w, h);

    row->text.text = text;

	return row;
}

gui_row* gui_row_create_separator(float x, float y, float w, float h)
{
    return gui_row_create(GUI_ROW_SEPARATOR, x, y, w, h);
}

void gui_row_free(gui_row* row)
{
    switch (row->type)
    {
    case GUI_ROW_TEXT: free(row->text.text); break;
    case GUI_ROW_BUTTON: break;
    }

    free(row);
}

void gui_row_render(const gui_window* window, gui_row* row, float x, float y, gui_theme theme)
{
    switch (row->type)
    {
    case GUI_ROW_SEPARATOR: Primitives2DFillRect(x + row->x, y + row->y + (row->h * 0.5f), window->w - 2.0f * window->padding, theme.separator_line_width, theme.font_color); break;
    }
}

void gui_row_render_font(const gui_window* window, gui_row* row, float x, float y, gui_theme theme)
{
    switch (row->type)
    {
    case GUI_ROW_TEXT: FontRendererRenderText(x + row->x, y + row->y + row->h, row->text.text); break;
    }
}
