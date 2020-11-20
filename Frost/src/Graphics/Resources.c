#include "Resources.h"

#include "toolbox/tb_json.h"

#include "math/math.h"

#include "Application/Debugger.h"
#include "Application/defines.h"

int ResourcesInit(Resources* resources, const char* path)
{
	resources->arena.blocks = NULL;

	if (tb_hashmap_alloc(&resources->textures, tb_hash_string, tb_hashmap_str_cmp, 0) != TB_HASHMAP_OK) return 0;
	if (tb_hashmap_alloc(&resources->fonts, tb_hash_string, tb_hashmap_str_cmp, 0) != TB_HASHMAP_OK) return 0;

	tb_hashmap_set_key_alloc_funcs(&resources->textures, tb_hashmap_str_alloc, tb_hashmap_str_free);
	tb_hashmap_set_key_alloc_funcs(&resources->fonts, tb_hashmap_str_alloc, tb_hashmap_str_free);

	char* json = ignisReadFile(path, NULL);

	if (!json)
	{
		DEBUG_ERROR("[Resources] Failed to read index (%s)", path);
		return 0;
	}

	/* textures */
	tb_json_element textures;
	tb_json_read(json, &textures, "{'textures'");

	for (int i = 0; i < textures.elements; i++)
	{
		char texture_name[APPLICATION_STR_LEN];
		tb_json_string((char*)textures.value, "{*", texture_name, APPLICATION_STR_LEN, &i);

		tb_json_element texture;
		tb_json_read_format((char*)textures.value, &texture, "{'%s'", texture_name);

		char texture_path[APPLICATION_PATH_LEN];
		tb_json_string((char*)texture.value, "{'path'", texture_path, APPLICATION_PATH_LEN, NULL);

		int rows = tb_json_int((char*)texture.value, "{'atlas'[0", NULL, 1);
		int columns = tb_json_int((char*)texture.value, "{'atlas'[1", NULL, 1);

		ResourcesAddTexture2D(resources, texture_name, texture_path, rows, columns);
	}

	/* fonts */
	tb_json_element fonts;
	tb_json_read(json, &fonts, "{'fonts'");

	for (int i = 0; i < fonts.elements; i++)
	{
		char font_name[APPLICATION_STR_LEN];
		tb_json_string((char*)fonts.value, "{*", font_name, APPLICATION_STR_LEN, &i);

		tb_json_element font;
		tb_json_read_format((char*)fonts.value, &font, "{'%s'", font_name);

		char font_path[APPLICATION_PATH_LEN];
		tb_json_string((char*)font.value, "{'path'", font_path, APPLICATION_PATH_LEN, NULL);

		float font_size = tb_json_float((char*)font.value, "{'size'", NULL, 0.0f);

		ResourcesAddFont(resources, font_name, font_path, font_size);
	}

	free(json);

	return 1;
}

void ResourcesDestroy(Resources* resources)
{
	for (tb_hashmap_iter* iter = tb_hashmap_iterator(&resources->textures); iter; iter = tb_hashmap_iter_next(&resources->textures, iter))
	{
		IgnisTexture2D* tex = tb_hashmap_iter_get_value(iter);
		ignisDeleteTexture2D(tex);
	}
	tb_hashmap_free(&resources->textures);

	for (tb_hashmap_iter* iter = tb_hashmap_iterator(&resources->fonts); iter; iter = tb_hashmap_iter_next(&resources->fonts, iter))
	{
		IgnisFont* font = tb_hashmap_iter_get_value(iter);
		ignisDeleteFont(font);
	}
	tb_hashmap_free(&resources->fonts);

	arena_free(&resources->arena);
}

IgnisTexture2D* ResourcesAddTexture2D(Resources* resources, const char* name, const char* path, int rows, int columns)
{
	IgnisTexture2D* texture = arena_alloc(&resources->arena, sizeof(IgnisTexture2D));
	if (ignisCreateTexture2D(texture, path, rows, columns, 1, NULL))
	{
		if (tb_hashmap_insert(&resources->textures, name, texture) == texture)
			return texture;

		DEBUG_ERROR("[Resources] Failed to add texture: %s (%s)\n", name, path);
		ignisDeleteTexture2D(texture);
	}

	return NULL;
}

IgnisFont* ResourcesAddFont(Resources* resources, const char* name, const char* path, float size)
{
	IgnisFont* font = arena_alloc(&resources->arena, sizeof(IgnisFont));
	if (ignisCreateFont(font, path, size))
	{
		if (tb_hashmap_insert(&resources->fonts, name, font) == font)
			return font;

		DEBUG_ERROR("[Resources] Failed to add font: %s (%s)\n", name, path);
		ignisDeleteFont(font);
	}
	return NULL;
}

IgnisTexture2D* ResourcesGetTexture2D(Resources* manager, const char* name)
{
	IgnisTexture2D* tex = tb_hashmap_find(&manager->textures, name);

	if (!tex) DEBUG_WARN("[Resources] Could not find texture: %s\n", name);

	return tex;
}

IgnisFont* ResourcesGetFont(Resources* manager, const char* name)
{
	IgnisFont* font = tb_hashmap_find(&manager->fonts, name);

	if (!font) DEBUG_WARN("[Resources] Could not find font: %s\n", name);

	return font;
}

const char* ResourcesGetTexture2DName(Resources* resources, IgnisTexture2D* texture)
{
	for (tb_hashmap_iter* iter = tb_hashmap_iterator(&resources->textures); iter; iter = tb_hashmap_iter_next(&resources->textures, iter))
	{
		if (texture == tb_hashmap_iter_get_value(iter))
			return tb_hashmap_iter_get_key(iter);
	}

	return NULL;
}

const char* ResourcesGetFontName(Resources* resources, IgnisFont* font)
{
	for (tb_hashmap_iter* iter = tb_hashmap_iterator(&resources->fonts); iter; iter = tb_hashmap_iter_next(&resources->fonts, iter))
	{
		if (font == tb_hashmap_iter_get_value(iter))
			return tb_hashmap_iter_get_key(iter);
	}

	return NULL;
}
