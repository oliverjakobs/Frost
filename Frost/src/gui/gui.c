#include "gui.h"

#include "clib/clib.h"
#include "clib/vector.h"

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
    gui_bg_style bg_style;

    float padding;

    gui_row* rows;
    size_t row_index;
    size_t row_capacity;
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

CLIB_VECTOR_DEFINE_FUNCS(gui_window, gui_window)

typedef struct
{
    clib_vector windows;

    float width;
    float height;

    gui_theme theme;
} gui_context;

static gui_context _context;
static gui_window* _current;

static void _gui_window_reset(gui_window* window)
{
    window->x = 0.0f;
    window->y = 0.0f;
    window->w = 0.0f;
    window->h = 0.0f;

    window->v_align = GUI_VALIGN_NONE;
    window->h_align = GUI_HALIGN_NONE;
    window->bg_style = GUI_BG_NONE;

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

void gui_init(float width, float height)
{
    clib_vector_init(&_context.windows, 4);

    _gui_load_theme(&_context.theme);
    _context.width = width;
    _context.height = height;

    _current = NULL;
}

void gui_free()
{
    clib_vector_free(&_context.windows);
}

void gui_set_font(IgnisFont* font, IgnisColorRGBA color)
{
    _context.theme.font = font;
    _context.theme.font_color = color;
}

void gui_start()
{
    // _gui_window_reset(&_context.windows[_context.window_index]);
}

void gui_render(const float* proj_mat)
{
    float w = 1.0f;
    glGetFloatv(GL_LINE_WIDTH, &w);

    glLineWidth(_context.theme.border_width);

    Primitives2DStart(proj_mat);
    for (size_t i = 0; i < _context.windows.size; i++)
    {
        gui_window* window = gui_window_vector_get(&_context.windows, i);

        if (window->bg_style != GUI_BG_NONE)
        {
            Primitives2DFillRect(window->x, window->y, window->w, window->h, _context.theme.bg_color);
            Primitives2DRenderRect(window->x, window->y, window->w, window->h, _context.theme.border_color);
        }
    }
    Primitives2DFlush();
    glLineWidth(w);

    FontRendererBindFont(_context.theme.font, _context.theme.font_color);
    FontRendererStart(proj_mat);
    for (size_t i = 0; i < _context.windows.size; i++)
    {
        gui_window* window = gui_window_vector_get(&_context.windows, i);

        for (size_t row = 0; row < window->row_index; row++)
        {
            FontRendererRenderText(window->x + window->rows[row].x, window->y + window->rows[row].y + window->rows[row].h, window->rows[row].text);
        }

        /* Free window */
        for (size_t i = 0; i < window->row_index; i++)
            free(window->rows[i].text);
        free(window->rows);

        gui_window_vector_delete(&_context.windows, i);
        free(window);
    }
    FontRendererFlush();
}

gui_window* _gui_window_create(size_t rows)
{
    gui_window* window = (gui_window*)malloc(sizeof(gui_window));

    if (window)
    {
        window->x = 0.0f;
        window->y = 0.0f;
        window->w = 0.0f;
        window->h = 0.0f;

        window->v_align = GUI_VALIGN_NONE;
        window->h_align = GUI_HALIGN_NONE;
        window->bg_style = GUI_BG_NONE;

        window->padding = 0.0f;

        window->rows = (gui_row*)malloc(sizeof(gui_row) * rows);
        window->row_capacity = rows;
        window->row_index = 0;
        window->row_y = 0.0f;
    }

    return window;
}

void gui_begin(float x, float y, float padding, int rows, gui_bg_style bg_style)
{
    if (_current) return;

    _current = _gui_window_create(rows);

    _current->x = x;
    _current->y = y;
    _current->w = padding;
    _current->h = padding;

    _current->bg_style = bg_style;

    _current->row_y = padding;
    _current->padding = padding;
}

void gui_begin_align(gui_halign h_align, gui_valign v_align, float padding, int rows, gui_bg_style bg_style)
{
    if (_current) return;

    _current = _gui_window_create(rows);

    _current->h_align = h_align;
    _current->v_align = v_align;
    _current->w = padding;
    _current->h = padding;

    _current->bg_style = bg_style;

    _current->row_y = padding;
    _current->padding = padding;
}

void gui_end()
{
    if (!_current) return;

    switch (_current->v_align)
    {
    case GUI_VALIGN_TOP:    _current->y = 0.0f; break;
    case GUI_VALIGN_CENTER: _current->y = (_context.height - _current->h) / 2.0f; break;
    case GUI_VALIGN_BOTTOM: _current->y = _context.height - _current->h; break;
    }

    switch (_current->h_align)
    {
    case GUI_HALIGN_LEFT:   _current->x = 0.0f; break;
    case GUI_HALIGN_CENTER: _current->x = (_context.width - _current->w) / 2.0f; break;
    case GUI_HALIGN_RIGHT:  _current->x = _context.width - _current->w; break;
    }

    gui_window_vector_push(&_context.windows, _current);
    _current = NULL;
}

gui_row* gui_row_init(float x, float y, float w, float h)
{
    gui_row* row = &_current->rows[_current->row_index];

    row->x = x;
    row->y = y;
    row->w = w;
    row->h = h;

    return row;
}

void gui_text(const char* fmt, ...)
{
    if (!_current) return;

    va_list args;
    va_start(args, fmt);
    size_t buffer_size = vsnprintf(NULL, 0, fmt, args);
    char* text = (char*)malloc(buffer_size + 1);
    vsnprintf(text, buffer_size + 1, fmt, args);
    va_end(args);

    float row_width = ignisFontGetTextWidth(_context.theme.font, text);
    float row_height = ignisFontGetTextHeight(_context.theme.font, text, NULL);

    gui_row* row = gui_row_init(_current->padding, _current->row_y, row_width, row_height);

    row->text = text;

    _current->w = MAX(_current->w, row_width + (2.0f * _current->padding));
    _current->h += row_height + _current->padding;

    _current->row_y += row_height + _current->padding;
    _current->row_index++;
}

