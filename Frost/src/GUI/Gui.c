#include "Gui.h"

#include "toolbox/tb_json.h"
#include "toolbox/tb_file.h"
#include "toolbox/tb_str.h"

#include "Application/Logger.h"
#include "Application/Application.h"

int GuiInit(GuiManager* gui, float w, float h, const char* path)
{
	char* json = tb_file_read(path, "rb");

	if (!json)
	{
		DEBUG_ERROR("[GUI] Failed to read index (%s)\n", path);
		return 0;
	}

	gui->arena.blocks = NULL;
	gui->arena.ptr = NULL;
	gui->arena.end = NULL;

	if (tb_hashmap_alloc(&gui->fonts, tb_hash_string, strcmp, 0) != TB_HASHMAP_OK)
	{
		DEBUG_ERROR("[GUI] Failed to allocate hashmap index\n");
		return 0;
	}

	tb_hashmap_set_key_alloc_funcs(&gui->fonts, tb_strdup, free);

	/* load fonts */
	tb_json_element fonts;
	tb_json_read(json, &fonts, NULL);

	for (int i = 0; i < fonts.elements; i++)
	{
		char font_name[APPLICATION_STR_LEN];
		tb_json_string(fonts.value, "{*", font_name, APPLICATION_STR_LEN, &i);

		tb_json_element font;
		tb_json_read_format(fonts.value, &font, "{'%s'", font_name);

		char font_path[APPLICATION_PATH_LEN];
		tb_json_string(font.value, "{'path'", font_path, APPLICATION_PATH_LEN, NULL);

		float font_size = tb_json_float(font.value, "{'size'", NULL, 0.0f);

		GuiAddFont(gui, font_name, font_path, font_size);
	}

	free(json);

	GuiSetViewport(gui, w, h);

	return 1;
}

void GuiDestroy(GuiManager* gui)
{
	for (tb_hashmap_iter* iter = tb_hashmap_iterator(&gui->fonts); iter; iter = tb_hashmap_iter_next(&gui->fonts, iter))
	{
		IgnisFont* font = tb_hashmap_iter_get_value(iter);
		ignisDeleteFont(font);
	}
	tb_hashmap_free(&gui->fonts);

	arena_free(&gui->arena);
}

IgnisFont* GuiAddFont(GuiManager* gui, const char* name, const char* path, float size)
{
	IgnisFont* font = arena_alloc(&gui->arena, sizeof(IgnisFont));
	if (ignisCreateFont(font, path, size))
	{
		if (tb_hashmap_insert(&gui->fonts, name, font) == font)
			return font;

		DEBUG_ERROR("[GUI] Failed to add font: %s (%s)\n", name, path);
		ignisDeleteFont(font);
	}
	return NULL;
}

IgnisFont* GuiGetFont(const GuiManager* gui, const char* name)
{
	IgnisFont* font = tb_hashmap_find(&gui->fonts, name);
	if (!font) DEBUG_WARN("[GUI] Could not find font: %s\n", name);
	return font;
}

const char* GuiGetFontName(const GuiManager* gui, const IgnisFont* font)
{
	for (tb_hashmap_iter* iter = tb_hashmap_iterator(&gui->fonts); iter; iter = tb_hashmap_iter_next(&gui->fonts, iter))
	{
		if (font == tb_hashmap_iter_get_value(iter))
			return tb_hashmap_iter_get_key(iter);
	}

	return NULL;
}

void GuiSetViewport(GuiManager* gui, float w, float h)
{
    gui->screen_projection = mat4_ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);
}

const float* GuiGetScreenProjPtr(const GuiManager* gui)
{
    return gui->screen_projection.v;
}
