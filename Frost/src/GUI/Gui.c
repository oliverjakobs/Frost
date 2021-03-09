#include "Gui.h"

void GuiSetViewport(GuiManager* gui, float w, float h)
{
    gui->screen_projection = mat4_ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);
}

const float* GuiGetScreenProjPtr(const GuiManager* gui)
{
    return gui->screen_projection.v;
}
