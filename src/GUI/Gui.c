#include "Gui.h"

#include "toolbox/tb_str.h"
#include "toolbox/tb_hashmap.h"

#include "Minimal/Minimal.h"

#include "Scenes/Scene.h"

typedef struct
{
	float width, height;
	mat4 screen_projection;

	tb_hashmap fonts;	/* <str,IgnisFont> */
} GuiManager;

static GuiManager _gui;

static int GuiAllocMapEntry(void* allocator, tb_hashmap_entry* entry, const void* key, void* value)
{
	entry->key = tb_mem_dup(allocator, key, strlen(key) + 1);
	entry->val = tb_mem_dup(allocator, value, sizeof(IgnisFont));

	return entry->key && entry->val;
}

static void GuiFreeMapEntry(void* allocator, tb_hashmap_entry* entry)
{
	ignisDeleteFont(entry->val);
	tb_mem_constfree(allocator, entry->key);
	tb_mem_free(allocator, entry->val);
}

int GuiInit(float w, float h, tb_allocator* allocator)
{
	_gui.fonts.allocator = allocator;
	_gui.fonts.alloc = tb_mem_calloc;
	_gui.fonts.free = tb_mem_free;
	_gui.fonts.entry_alloc = GuiAllocMapEntry;
	_gui.fonts.entry_free = GuiFreeMapEntry;
	if (tb_hashmap_init(&_gui.fonts, tb_hash_string, strcmp, 0) != TB_HASHMAP_OK)
	{
		MINIMAL_ERROR("[GUI] Failed to allocate hashmap index\n");
		return 0;
	}

	GuiSetViewport(w, h);

	return 1;
}

void GuiDestroy()
{
	tb_hashmap_destroy(&_gui.fonts);
}

IgnisFont* GuiAddFont(const char* name, const char* path, float size)
{
	IgnisFont font;
	if (ignisCreateFont(&font, path, size))
	{
		IgnisFont* entry = tb_hashmap_insert(&_gui.fonts, name, &font);
		if (entry != NULL) return entry;

		MINIMAL_ERROR("[GUI] Failed to add font: %s (%s)\n", name, path);
	}
	return NULL;
}

IgnisFont* GuiGetFont(const char* name)
{
	IgnisFont* font = tb_hashmap_find(&_gui.fonts, name);
	if (!font) MINIMAL_WARN("[GUI] Could not find font: %s\n", name);
	return font;
}

const char* GuiGetFontName(const IgnisFont* font)
{
	for (tb_hashmap_iter* iter = tb_hashmap_iterator(&_gui.fonts); iter; iter = tb_hashmap_iter_next(&_gui.fonts, iter))
	{
		if (font == tb_hashmap_iter_get_val(iter)) return tb_hashmap_iter_get_key(iter);
	}

	return NULL;
}

void GuiSetViewport(float w, float h)
{
	_gui.width = w;
	_gui.height = h;
    _gui.screen_projection = mat4_ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);
}

float GuiGetScreenWidth()			{ return _gui.width; }
float GuiGetScreenHeight()			{ return _gui.height; }
const float* GuiGetScreenProjPtr()	{ return _gui.screen_projection.v; }
