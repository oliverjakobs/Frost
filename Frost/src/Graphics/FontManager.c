#include "FontManager.h"

#include "toolbox/tb_json.h"
#include "toolbox/tb_file.h"

#include "Application/Logger.h"
#include "Application/Application.h"

int FontManagerInit(FontManager* manager, const char* path)
{
	char* json = tb_file_read(path, "rb", NULL);

	if (!json)
	{
		DEBUG_ERROR("[FontManager] Failed to read index (%s)\n", path);
		return 0;
	}

	manager->arena.blocks = NULL;
	manager->arena.ptr = NULL;
	manager->arena.end = NULL;

	if (tb_hashmap_alloc(&manager->fonts, tb_hash_string, tb_hashmap_str_cmp, 0) != TB_HASHMAP_OK)
	{
		DEBUG_ERROR("[FontManager] Failed to allocate hashmap index\n");
		return 0;
	}

	tb_hashmap_set_key_alloc_funcs(&manager->fonts, tb_hashmap_str_alloc, tb_hashmap_str_free);

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

		FontManagerAddFont(manager, font_name, font_path, font_size);
	}

	free(json);

	return 1;
}

void FontManagerDestroy(FontManager* manager)
{
	for (tb_hashmap_iter* iter = tb_hashmap_iterator(&manager->fonts); iter; iter = tb_hashmap_iter_next(&manager->fonts, iter))
	{
		IgnisFont* font = tb_hashmap_iter_get_value(iter);
		ignisDeleteFont(font);
	}
	tb_hashmap_free(&manager->fonts);

	arena_free(&manager->arena);
}

IgnisFont* FontManagerAddFont(FontManager* manager, const char* name, const char* path, float size)
{
	IgnisFont* font = arena_alloc(&manager->arena, sizeof(IgnisFont));
	if (ignisCreateFont(font, path, size))
	{
		if (tb_hashmap_insert(&manager->fonts, name, font) == font)
			return font;

		DEBUG_ERROR("[FontManager] Failed to add font: %s (%s)\n", name, path);
		ignisDeleteFont(font);
	}
	return NULL;
}

IgnisFont* FontManagerGetFont(const FontManager* manager, const char* name)
{
	IgnisFont* font = tb_hashmap_find(&manager->fonts, name);
	if (!font) DEBUG_WARN("[FontManager] Could not find font: %s\n", name);
	return font;
}

const char* FontManagerGetFontName(const FontManager* manager, const IgnisFont* font)
{
	for (tb_hashmap_iter* iter = tb_hashmap_iterator(&manager->fonts); iter; iter = tb_hashmap_iter_next(&manager->fonts, iter))
	{
		if (font == tb_hashmap_iter_get_value(iter))
			return tb_hashmap_iter_get_key(iter);
	}

	return NULL;
}
