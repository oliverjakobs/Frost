#include "gui.h"

#include "clib/clib.h"

typedef struct
{
    char* text;
    float x;
    float y;
    float w;
    float h;
} gui_row;

typedef struct
{
    float x;
    float y;
    float w;
    float h;

    gui_valign v_align;
    gui_halign h_align;
    gui_background background;

    float padding;

    gui_row* rows;
    size_t row_index;
    float row_y;
} gui_window;

typedef struct
{
    IgnisFont* font;
    IgnisColorRGBA font_color;
    IgnisColorRGBA bg_color;
    IgnisColorRGBA border_color;

    float border_width;
} gui_theme;

typedef struct
{
    gui_window windows[3];
    size_t window_index;

    gui_theme theme;
} gui_context;

static gui_context _context;

static void _gui_window_reset(gui_window* window)
{
    window->x = 0.0f;
    window->y = 0.0f;
    window->w = 0.0f;
    window->h = 0.0f;

    window->v_align = GUI_VALIGN_NONE;
    window->h_align = GUI_HALIGN_NONE;
    window->background = GUI_BG_NONE;

    window->padding = 0.0f;

    if (window->rows)
    {
        for (size_t i = 0; i < window->row_index; i++)
            free(window->rows[i].text);
        free(window->rows);
    }
    window->rows = NULL;
    window->row_index = 0;
    window->row_y = 0.0f;
}

static void _gui_load_theme(gui_theme* theme)
{
    theme->font = NULL;
    theme->font_color = IGNIS_WHITE;
    theme->bg_color = (IgnisColorRGBA){ 0.2f, 0.2f, 0.2f, 1.0f };
    theme->border_color = IGNIS_BLACK;

    theme->border_width = 2.0f;
}

void gui_init()
{
    _context.window_index = 0;

    _gui_load_theme(&_context.theme);
}

void gui_free()
{

}

void gui_set_font(IgnisFont* font, IgnisColorRGBA color)
{
    _context.theme.font = font;
    _context.theme.font_color = color;
}

void gui_start()
{
    _gui_window_reset(&_context.windows[_context.window_index]);
}

void gui_render(const float* proj_mat)
{
    float w = 1.0f;
    glGetFloatv(GL_LINE_WIDTH, &w);

    glLineWidth(_context.theme.border_width);

    for (size_t i = 0; i < _context.window_index; i++)
    {
        Primitives2DStart(proj_mat);

        gui_window* window = &_context.windows[i];

        if (window->background != GUI_BG_NONE)
        {
            Primitives2DFillRect(window->x, window->y, window->w, window->h, _context.theme.bg_color);
            Primitives2DRenderRect(window->x, window->y, window->w, window->h, _context.theme.border_color);
        }

        Primitives2DFlush();
    }
    glLineWidth(w);

    FontRendererBindFont(_context.theme.font, IGNIS_WHITE);
    for (size_t i = 0; i < _context.window_index; i++)
    {
        FontRendererStart(proj_mat);

        for (size_t row = 0; row < _context.windows[i].row_index; row++)
        {
            FontRendererRenderText(_context.windows[i].x + _context.windows[i].rows[row].x, _context.windows[i].y + _context.windows[i].rows[row].y + _context.windows[i].rows[row].h, _context.windows[i].rows[row].text);
        }

        FontRendererFlush();

        _gui_window_reset(&_context.windows[i]);
    }

    _context.window_index = 0;
}

void gui_begin(float x, float y, float padding, int rows, gui_background bg)
{
    _context.windows[_context.window_index].x = x;
    _context.windows[_context.window_index].y = y;
    _context.windows[_context.window_index].w = padding;
    _context.windows[_context.window_index].h = padding;

    _context.windows[_context.window_index].background = bg;

    _context.windows[_context.window_index].row_y = padding;

    _context.windows[_context.window_index].padding = padding;

    _context.windows[_context.window_index].rows = (gui_row*)malloc(sizeof(gui_row) * rows);
}

void gui_begin_align(gui_halign h_align, gui_valign v_align, float padding, int rows, gui_background bg)
{
    _context.windows[_context.window_index].h_align = h_align;
    _context.windows[_context.window_index].v_align = v_align;
    _context.windows[_context.window_index].w = padding;
    _context.windows[_context.window_index].h = padding;

    _context.windows[_context.window_index].background = bg;

    _context.windows[_context.window_index].row_y = padding;

    _context.windows[_context.window_index].padding = padding;

    _context.windows[_context.window_index].rows = (gui_row*)malloc(sizeof(gui_row) * rows);
}

static float SCREEN_WIDTH = 1024.0f;
static float SCREEN_HEIGHT = 800.0f;

void gui_end()
{
    switch (_context.windows[_context.window_index].v_align)
    {
    case GUI_VALIGN_TOP:    _context.windows[_context.window_index].y = 0.0f; break;
    case GUI_VALIGN_CENTER: _context.windows[_context.window_index].y = (SCREEN_HEIGHT - _context.windows[_context.window_index].h) / 2.0f; break;
    case GUI_VALIGN_BOTTOM: _context.windows[_context.window_index].y = SCREEN_HEIGHT - _context.windows[_context.window_index].h; break;
    }

    switch (_context.windows[_context.window_index].h_align)
    {
    case GUI_HALIGN_LEFT:   _context.windows[_context.window_index].x = 0.0f; break;
    case GUI_HALIGN_CENTER: _context.windows[_context.window_index].x = (SCREEN_WIDTH - _context.windows[_context.window_index].w) / 2.0f; break;
    case GUI_HALIGN_RIGHT:  _context.windows[_context.window_index].x = SCREEN_WIDTH - _context.windows[_context.window_index].w; break;
    }

    _context.window_index++;
}

void gui_text(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    size_t buffer_size = vsnprintf(NULL, 0, fmt, args);
    char* text = (char*)malloc(buffer_size + 1);
    vsnprintf(text, buffer_size + 1, fmt, args);
    va_end(args);

    _context.windows[_context.window_index].rows[_context.windows[_context.window_index].row_index].text = text;

    _context.windows[_context.window_index].rows[_context.windows[_context.window_index].row_index].x = _context.windows[_context.window_index].padding;
    _context.windows[_context.window_index].rows[_context.windows[_context.window_index].row_index].y = _context.windows[_context.window_index].row_y;

    float row_width = ignisFontGetTextWidth(_context.theme.font, text);
    float row_height = ignisFontGetTextHeight(_context.theme.font, text, NULL);

    _context.windows[_context.window_index].rows[_context.windows[_context.window_index].row_index].w = row_width;
    _context.windows[_context.window_index].rows[_context.windows[_context.window_index].row_index].h = row_height;

    _context.windows[_context.window_index].w = MAX(_context.windows[_context.window_index].w, row_width + (2.0f * _context.windows[_context.window_index].padding));
    _context.windows[_context.window_index].h += row_height + _context.windows[_context.window_index].padding;

    _context.windows[_context.window_index].row_y += row_height + _context.windows[_context.window_index].padding;
    _context.windows[_context.window_index].row_index++;
}

