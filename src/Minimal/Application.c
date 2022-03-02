#include "Application.h"

#include <string.h>

/* --------------------------| minimal app |----------------------------- */
static void MinimalGetGLVersion(const char* version_str, int* major, int* minor)
{
    const char* sep = ".";
    const char* major_str = version_str;
    const char* minor_str = version_str + strcspn(version_str, sep) + 1;

    if (major_str && major) *major = atoi(major_str);
    if (minor_str && minor) *minor = atoi(minor_str);
}

void MinimalGLFWErrorCallback(int error, const char* desc)
{
    MINIMAL_ERROR("[GLFW] (%d) %s", error, desc);
}

void MinimalGLFWWindowSizeCallback(GLFWwindow* window, int width, int height);

void MinimalGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MinimalGLFWCharCallback(GLFWwindow* window, unsigned int keycode);
void MinimalGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void MinimalGLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void MinimalGLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

int MinimalLoad(MinimalApp* app, const char* title, uint32_t w, uint32_t h, const char* gl)
{
    app->debug = 0;
    app->vsync = 0;

    /* minimal initialization */
    if (!glfwInit())
    {
        MINIMAL_ERROR("[GLFW] Failed to initialize GLFW");
        return MINIMAL_FAIL;
    }

    int gl_major, gl_minor;
    MinimalGetGLVersion(gl, &gl_major, &gl_minor);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

    glfwSetErrorCallback(MinimalGLFWErrorCallback);

    /* creating the window */
    app->window = glfwCreateWindow(w, h, title, NULL, NULL);
    if (app->window == NULL)
    {
        MINIMAL_ERROR("[GLFW] Failed to create GLFW window");
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(app->window);
    glfwSetWindowUserPointer(app->window, app);

    /* Set GLFW callbacks */
    glfwSetWindowSizeCallback(app->window, MinimalGLFWWindowSizeCallback);
    glfwSetKeyCallback(app->window, MinimalGLFWKeyCallback);
    glfwSetCharCallback(app->window, MinimalGLFWCharCallback);
    glfwSetMouseButtonCallback(app->window, MinimalGLFWMouseButtonCallback);
    glfwSetCursorPosCallback(app->window, MinimalGLFWCursorPosCallback);
    glfwSetScrollCallback(app->window, MinimalGLFWScrollCallback);

    MinimalTimerReset(&app->timer);

    return (app->on_load) ? app->on_load(app, w, h) : MINIMAL_OK;
}

void MinimalDestroy(MinimalApp* app)
{
    if (app->on_destroy) app->on_destroy(app);
    glfwDestroyWindow(app->window);
}

void MinimalRun(MinimalApp* app, void(*clear_buffer)())
{
    while (!glfwWindowShouldClose(app->window))
    {
        MinimalTimerStart(&app->timer, glfwGetTime());
        // MinimalWindowUpdateKeyStates(app->window);

        app->on_update(app, (float)app->timer.deltatime);

        if (clear_buffer) clear_buffer();

        app->on_render(app);

        if (app->debug) app->on_render_debug(app);

        app->on_render_ui(app);

        glfwPollEvents();
        glfwSwapBuffers(app->window);

        MinimalTimerEnd(&app->timer, glfwGetTime());
    }
}

void MinimalClose(MinimalApp* app) { glfwSetWindowShouldClose(app->window, GLFW_TRUE); }

MinimalLoadCB MinimalSetLoadCallback(MinimalApp* app, MinimalLoadCB callback) 
{
    MinimalLoadCB prev = app->on_load;
    app->on_load = callback;
    return prev;
}

MinimalDestroyCB MinimalSetDestroyCallback(MinimalApp* app, MinimalDestroyCB callback)
{
    MinimalDestroyCB prev = app->on_destroy;
    app->on_destroy = callback;
    return prev;
}

MinimalEventCB MinimalSetEventCallback(MinimalApp* app, MinimalEventCB callback)
{
    MinimalEventCB prev = app->on_event;
    app->on_event = callback;
    return prev;
}

MinimalUpdateCB MinimalSetUpdateCallback(MinimalApp* app, MinimalUpdateCB callback)
{
    MinimalUpdateCB prev = app->on_update;
    app->on_update = callback;
    return prev;
}

MinimalRenderCB MinimalSetRenderCallback(MinimalApp* app, MinimalRenderCB callback)
{
    MinimalRenderCB prev = app->on_render;
    app->on_render = callback;
    return prev;
}

MinimalRenderCB MinimalSetRenderDebugCallback(MinimalApp* app, MinimalRenderCB callback)
{
    MinimalRenderCB prev = app->on_render_debug;
    app->on_render_debug = callback;
    return prev;
}

MinimalRenderCB MinimalSetRenderUICallback(MinimalApp* app, MinimalRenderCB callback)
{
    MinimalRenderCB prev = app->on_render_ui;
    app->on_render_ui = callback;
    return prev;
}

/* --------------------------| settings |-------------------------------- */
void MinimalSetWindowTitle(MinimalApp* app, const char* title) { glfwSetWindowTitle(app->window, title); }

void MinimalEnableDebug(MinimalApp* app, int b) { app->debug = b; }
void MinimalEnableVsync(MinimalApp* app, int b) { glfwSwapInterval(b); app->vsync = b; }

void MinimalToggleDebug(MinimalApp* app) { MinimalEnableDebug(app, !app->debug); }
void MinimalToggleVsync(MinimalApp* app) { MinimalEnableVsync(app, !app->vsync); }

/* --------------------------| glfw events |----------------------------- */
#define MINIMAL_MAKE_MOUSE_U(bc, a) ((bc << 16) + a)

static void MinimalDispatchGLFWEvent(GLFWwindow* window, uint32_t type, uint32_t uParam, int32_t lParam, int32_t rParam)
{
    MinimalApp* app = (MinimalApp*)glfwGetWindowUserPointer(window);
    if (app) MinimalDispatchEvent(app, type, uParam, lParam, rParam);
}

void MinimalGLFWWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    MinimalDispatchGLFWEvent(window, MINIMAL_EVENT_WINDOW_SIZE, 0, width, height);
}

void MinimalGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    MinimalDispatchGLFWEvent(window, MINIMAL_EVENT_KEY, key, action, mods);
}

void MinimalGLFWCharCallback(GLFWwindow* window, unsigned int keycode)
{
    MinimalDispatchGLFWEvent(window, MINIMAL_EVENT_CHAR, keycode, 0, 0);
}

void MinimalGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    MinimalDispatchGLFWEvent(window, MINIMAL_EVENT_MOUSE_BUTTON, MINIMAL_MAKE_MOUSE_U(button, action), (int32_t)xpos, (int32_t)ypos);
}

void MinimalGLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    MinimalDispatchGLFWEvent(window, MINIMAL_EVENT_MOUSE_BUTTON, 0, (int32_t)xpos, (int32_t)ypos);
}

void MinimalGLFWScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    MinimalDispatchGLFWEvent(window, MINIMAL_EVENT_MOUSE_SCROLLED, 0, (int32_t)xOffset, (int32_t)yOffset);
}