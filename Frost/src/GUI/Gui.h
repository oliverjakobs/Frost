#ifndef GUI_H
#define GUI_H

#include "Camera/Camera.h"

typedef struct
{
    mat4 screen_projection;
} GuiManager;

void GuiSetViewport(GuiManager* gui, float w, float h);
const float* GuiGetScreenProjPtr(const GuiManager* gui);

#endif // !GUI_H
