#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "Graphics/Renderer.h"

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
	GUI_BG_BORDER,
} gui_background;

void gui_init();
void gui_free();

void gui_set_font(IgnisFont* font, IgnisColorRGBA color);

void gui_start();
void gui_render(const float* proj_mat);

void gui_begin(float x, float y, float padding, int rows, gui_background bg);
void gui_begin_align(gui_halign h_align, gui_valign v_align, float padding, int rows, gui_background bg);
void gui_end();

void gui_text(const char* text, ...);


#ifdef __cplusplus
}
#endif