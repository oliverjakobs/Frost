#include "gui.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"
#include "nuklear_glfw_gl3.h"

static struct nk_context* _context;

void gui_init(GLFWwindow* window)
{
    _context = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS);

    struct nk_font_atlas* atlas;
    nk_glfw3_font_stash_begin(&atlas);
    nk_glfw3_font_stash_end();
}

void gui_free()
{
    nk_glfw3_shutdown();
}

void gui_start()
{
    nk_glfw3_new_frame();
}

void gui_render()
{
    nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

int gui_begin(const char* title, float x, float y, float w, float h)
{
    return nk_begin(_context, title, nk_rect(x, y, w, h), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE);
}

void gui_end()
{
    nk_end(_context);
}

void gui_demo()
{
    if (gui_begin("Demo", 50, 50, 230, 250))
    {
        enum { EASY, HARD };
        static int op = EASY;
        static int property = 20;
        nk_layout_row_static(_context, 30, 80, 1);
        if (nk_button_label(_context, "button"))
            fprintf(stdout, "button pressed\n");

        nk_layout_row_dynamic(_context, 30, 2);
        if (nk_option_label(_context, "easy", op == EASY)) op = EASY;
        if (nk_option_label(_context, "hard", op == HARD)) op = HARD;

        nk_layout_row_dynamic(_context, 200, 1);
        nk_property_int(_context, "Compression:", 0, &property, 100, 10, 1);
    }
    gui_end();
}

