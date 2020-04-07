#ifndef GUI_ROW_H
#define GUI_ROW_H

typedef struct
{
    char* text;
    float x;
    float y;
    float w;
    float h;
} gui_row;

gui_row* gui_row_create_text(char* text, float x, float y, float w, float h);

void gui_row_free(gui_row* row);


#endif /* !GUI_ROW_H */
