#ifndef GUI_H
#define GUI_H

#include "Camera/Camera.h"
#include "Ignis/Ignis.h"

#include "toolbox/tb_hashmap.h"
#include "memory/arena.h"

typedef struct
{
	mat4 screen_projection;
	
	tb_hashmap fonts;	/* <str,IgnisFont> */
	Arena arena;
} GuiManager;


int GuiInit(GuiManager* gui, float w, float h, const char* path);
void GuiDestroy(GuiManager* gui);

IgnisFont* GuiAddFont(GuiManager* gui, const char* name, const char* path, float size);

IgnisFont* GuiGetFont(const GuiManager* gui, const char* name);
const char* GuiGetFontName(const GuiManager* gui, const IgnisFont* font);

void GuiSetViewport(GuiManager* gui, float w, float h);
const float* GuiGetScreenProjPtr(const GuiManager* gui);

#endif // !GUI_H
