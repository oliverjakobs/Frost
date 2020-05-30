#include "gui.h"

CLIB_VECTOR_DEFINE_FUNCS(gui_row, gui_row)
CLIB_VECTOR_DEFINE_FUNCS(gui_window, gui_window)

static gui_context _context;
static gui_window* _current;

static void _gui_load_theme(gui_theme* theme)
{
    theme->font = NULL;
    theme->font_color = IGNIS_WHITE;
    theme->bg_color = (IgnisColorRGBA){ 0.2f, 0.2f, 0.2f, 1.0f };
    theme->border_color = IGNIS_BLACK;

    theme->border_width = 2.0f;
    theme->separator_line_width = 2.0f;
    theme->separator_height = 8.0f;
}

void gui_init(float width, float height)
{
    clib_vector_init(&_context.windows, GUI_INITIAL_WINDOWS);

    _gui_load_theme(&_context.theme);
    _context.width = width;
    _context.height = height;

    _current = NULL;
}

void gui_free()
{
    for (size_t i = 0; i < _context.windows.size; i++)
    {
        gui_window* window = gui_window_vector_get(&_context.windows, i);

        for (size_t row_i = 0; row_i < window->rows.size; row_i++)
        {
            gui_row* row = gui_row_vector_get(&window->rows, row_i);
            gui_row_free(row);
        }

        gui_window_vector_delete(&_context.windows, i);
        clib_vector_free(&window->rows);
        free(window);
    }

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
    for (size_t i = 0; i < _context.windows.size; ++i)
    {
        gui_window* window = gui_window_vector_get(&_context.windows, i);

        if (window->bg_style != GUI_BG_NONE)
        {
            Primitives2DFillRect(window->x, window->y, window->w, window->h, _context.theme.bg_color);
            Primitives2DRenderRect(window->x, window->y, window->w, window->h, _context.theme.border_color);
        }

        for (size_t row_i = 0; row_i < window->rows.size; ++row_i)
        {
            gui_row* row = gui_row_vector_get(&window->rows, row_i);
            gui_row_render(window, row, _context.theme);
        }
    }
    Primitives2DFlush();
    glLineWidth(w);

    FontRendererStart(proj_mat);
    FontRendererBindFont(_context.theme.font, _context.theme.font_color);
    for (size_t i = 0; i < _context.windows.size; ++i)
    {
        gui_window* window = gui_window_vector_get(&_context.windows, i);

        for (size_t row_i = 0; row_i < window->rows.size; ++row_i)
        {
            gui_row* row = gui_row_vector_get(&window->rows, row_i);
            gui_row_render_font(window, row, _context.theme);
            gui_row_free(row);
        }

        gui_window_vector_delete(&_context.windows, i);
        clib_vector_free(&window->rows);
        free(window);
    }
    FontRendererFlush();
}

int gui_begin(float x, float y, float w, float h, float padding, gui_bg_style bg_style)
{
    if (_current) return 0;

    gui_window* window = (gui_window*)malloc(sizeof(gui_window));

    if (window)
    {
        window->x = x;
        window->y = y;
        window->w = w;
        window->h = h;

        window->h_align = GUI_HALIGN_NONE;
        window->v_align = GUI_HALIGN_NONE;
        window->bg_style = bg_style;

        window->padding = padding;

        clib_vector_init(&window->rows, GUI_INITIAL_ROWS);
        window->row_y = padding;

        _current = window;

        return 1;
    }

    return 0;
}

int gui_begin_align(gui_halign h_align, gui_valign v_align, float w, float h, float padding, gui_bg_style bg_style)
{
    if (gui_begin(0.0f, 0.0f, w, h, padding, bg_style))
    {
        switch (v_align)
        {
        case GUI_VALIGN_TOP:    _current->y = 0.0f; break;
        case GUI_VALIGN_CENTER: _current->y = (_context.height - _current->h) / 2.0f; break;
        case GUI_VALIGN_BOTTOM: _current->y = _context.height - _current->h; break;
        }

        switch (h_align)
        {
        case GUI_HALIGN_LEFT:   _current->x = 0.0f; break;
        case GUI_HALIGN_CENTER: _current->x = (_context.width - _current->w) / 2.0f; break;
        case GUI_HALIGN_RIGHT:  _current->x = _context.width - _current->w; break;
        }

        return 1;
    }

    return 0;
}

void gui_end()
{
    if (!_current) return;

    gui_window_vector_push(&_context.windows, _current);
    _current = NULL;
}

static void _gui_add_row(gui_row* row)
{
    row->x += _current->x;
    row->y += _current->y;

    _current->row_y += row->h + _current->padding;

    gui_row_vector_push(&_current->rows, row);
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

    float row_width = ignisFontGetTextWidth(_context.theme.font, text, strlen(text));
    float row_height = ignisFontGetTextHeight(_context.theme.font, text, strlen(text), NULL);

    gui_row* row = gui_row_create_text(text, _current->padding, _current->row_y, row_width, row_height);

    _gui_add_row(row);
}

void gui_separator()
{
    gui_row* row = gui_row_create_separator(_current->padding, _current->row_y, _current->w - 2.0f * _current->padding, _context.theme.separator_height);

    _gui_add_row(row);
}

void gui_checkbox(const char* text, int* state)
{
}

#include "Application/Input.h"

int gui_button(const char* text)
{
    float padding = _current->padding;

    float row_width = ignisFontGetTextWidth(_context.theme.font, text, strlen(text)) + (2.0f * padding);
    float row_height = ignisFontGetTextHeight(_context.theme.font, text, strlen(text), NULL) + (2.0f * padding);

    gui_row* row = gui_row_create_button((char*)text, _current->padding, _current->row_y, row_width, row_height, padding);

    _gui_add_row(row);

    vec2 mouse_pos = InputMousePositionVec2();

    if (vec2_inside(mouse_pos, (vec2) { row->x, row->y }, (vec2) { row->x + row->w, row->y + row->h }))
    {
        row->button.state = 1;
        return InputMousePressed(MOUSE_BUTTON_LEFT);
    }

    return 0;
}

