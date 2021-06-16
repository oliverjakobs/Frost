#include "Gui.h"

#include "toolbox/tb_json.h"
#include "toolbox/tb_ini.h"
#include "toolbox/tb_file.h"
#include "toolbox/tb_str.h"

#include "Minimal/Minimal.h"

#include "Scenes/Scene.h"

static int GuiAllocMapEntry(void* allocator, tb_hashmap_entry* entry, void* key, void* value)
{
	entry->key = tb_mem_dup(allocator, key, strlen(key) + 1);

	if (!entry->key) return 0;

	entry->value = tb_mem_dup(allocator, value, sizeof(IgnisFont));

	return entry->value != NULL;
}

static void GuiFreeMapEntry(void* allocator, tb_hashmap_entry* entry)
{
	tb_mem_free(allocator, entry->key);
	ignisDeleteFont(entry->value);
	tb_mem_free(allocator, entry->value);
}

int GuiInit(GuiManager* gui, float w, float h, const char* path, tb_allocator* allocator)
{
	char* config = tb_file_read(path, "rb");
	if (!config)
	{
		MINIMAL_ERROR("[GUI] Failed to read config (%s)\n", path);
		return 0;
	}

	gui->fonts.allocator = allocator;
	gui->fonts.alloc = tb_mem_calloc;
	gui->fonts.free = tb_mem_free;
	gui->fonts.entry_alloc = GuiAllocMapEntry;
	gui->fonts.entry_free = GuiFreeMapEntry;
	if (tb_hashmap_init(&gui->fonts, tb_hash_string, strcmp, 0) != TB_HASHMAP_OK)
	{
		MINIMAL_ERROR("[GUI] Failed to allocate hashmap index\n");
		return 0;
	}

	/* load fonts */
	tb_ini_element font;
	char* cursor = config;
	while ((cursor = tb_ini_query_group(cursor, "font", &font)) != NULL)
	{
		char font_name[APPLICATION_STR_LEN];
		tb_ini_name(&font, font_name, APPLICATION_STR_LEN);

		char font_path[APPLICATION_PATH_LEN];
		tb_ini_query_string(font.start, NULL, "path", font_path, APPLICATION_STR_LEN);

		float font_size = tb_ini_query_float(font.start, NULL, "size", 0.0f);
		GuiAddFont(gui, font_name, font_path, font_size);
	}

	free(config);

	GuiSetViewport(gui, w, h);

	return 1;
}

void GuiDestroy(GuiManager* gui)
{
	tb_hashmap_destroy(&gui->fonts);
}

IgnisFont* GuiAddFont(GuiManager* gui, const char* name, const char* path, float size)
{
	IgnisFont font;
	if (ignisCreateFont(&font, path, size))
	{
		IgnisFont* entry = tb_hashmap_insert(&gui->fonts, name, &font);
		if (entry != NULL) return entry;

		MINIMAL_ERROR("[GUI] Failed to add font: %s (%s)\n", name, path);
		ignisDeleteFont(&font);
	}
	return NULL;
}

IgnisFont* GuiGetFont(const GuiManager* gui, const char* name)
{
	IgnisFont* font = tb_hashmap_find(&gui->fonts, name);
	if (!font) MINIMAL_WARN("[GUI] Could not find font: %s\n", name);
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
	gui->width = w;
	gui->height = h;
    gui->screen_projection = mat4_ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);
}

const float* GuiGetScreenProjPtr(const GuiManager* gui)
{
    return gui->screen_projection.v;
}
