#include "row.h"

#include <stdlib.h>

gui_row* gui_row_create(float x, float y, float w, float h)
{
    gui_row* row = (gui_row*)malloc(sizeof(gui_row));

    row->x = x;
    row->y = y;
    row->w = w;
    row->h = h;

    return row;
}

gui_row* gui_row_create_text(char* text, float x, float y, float w, float h)
{
    gui_row* row = gui_row_create(x, y, w, h);
    row->text = text;

	return row;
}

void gui_row_free(gui_row* row)
{
    free(row->text);
    free(row);
}
