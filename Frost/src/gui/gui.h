#ifndef GUI_H
#define GUI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "row.h"

#include "Graphics/Renderer.h"

void gui_init(float width, float height);
void gui_free();

void gui_set_font(IgnisFont* font, IgnisColorRGBA color);

void gui_start();
void gui_render(const float* proj_mat);

int gui_begin(float x, float y, float w, float h, float padding, gui_bg_style bg_style);
int gui_begin_align(gui_halign h_align, gui_valign v_align, float w, float h, float padding, gui_bg_style bg_style);
void gui_end();

void gui_text(const char* text, ...);
void gui_separator();
void gui_checkbox(const char* text, int* state);

int gui_button(const char* text);


#ifdef __cplusplus
}
#endif

#endif /* !GUI_H */