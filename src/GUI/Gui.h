#ifndef GUI_H
#define GUI_H

#include "math/math.h"
#include "Ignis/Ignis.h"

#include "toolbox/tb_mem.h"

int GuiInit(float w, float h, tb_allocator* allocator);
void GuiDestroy();

IgnisFont* GuiAddFont(const char* name, const char* path, float size);

IgnisFont* GuiGetFont(const char* name);
const char* GuiGetFontName(const IgnisFont* font);

void GuiSetViewport(float w, float h);

float GuiGetScreenWidth();
float GuiGetScreenHeight();
const float* GuiGetScreenProjPtr();

#endif // !GUI_H
