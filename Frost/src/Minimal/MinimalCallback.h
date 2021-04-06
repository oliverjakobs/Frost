#ifndef MINIMAL_CALLBACK_H
#define MINIMAL_CALLBACK_H

#include "Minimal.h"

/*
 * Gets called when an event is processed, handles some (e.g. WindowCloseEvents) 
 * and passes the rest to the on_event callback of app
 */
void MinimalEventCallback(MinimalApp* app, Event e);

/* Translates GLFW events to internal events */
void MinimalGLFWErrorCallback(int error, const char* desc);
void MinimalGLFWWindowSizeCallback(GLFWwindow* window, int width, int height);
void MinimalGLFWWindowCloseCallback(GLFWwindow* window);
void MinimalGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MinimalGLFWCharCallback(GLFWwindow* window, unsigned int keycode);
void MinimalGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void MinimalGLFWScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void MinimalGLFWCursorPosCallback(GLFWwindow* window, double xPos, double yPos);

#endif /* !MINIMAL_CALLBACK_H */
