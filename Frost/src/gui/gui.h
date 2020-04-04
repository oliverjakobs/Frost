#pragma once


#ifdef __cplusplus
extern "C"
{
#endif

#include "Ignis/Packages/glad/glad.h"
#include <GLFW/glfw3.h>

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

void gui_init(GLFWwindow* window);
void gui_free();

void gui_start();
void gui_render();

int gui_begin(const char* title, float x, float y, float w, float h);
void gui_end();

void gui_demo();


#ifdef __cplusplus
}
#endif