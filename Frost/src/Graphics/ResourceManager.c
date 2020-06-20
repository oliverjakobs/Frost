#include "ResourceManager.h"

#define TB_JSON_IMPLEMENTATION
#include "json/tb_json.h"

#include "math/math.h"

#include "Application/Debugger.h"
#include "Application/defines.h"

int ResourceManagerInit(ResourceManager* resources, const char* path)
{
	clib_dict_alloc(&resources->textures, 0);
	clib_dict_alloc(&resources->fonts, 0);

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

		ResourceManagerAddTexture2D(resources, texture_name, texture_path, rows, columns);
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

		ResourceManagerAddFont(resources, font_name, font_path, font_size);
	}

	free(json);

	return 1;
}

void ResourceManagerDestroy(ResourceManager* resources)
{
	CLIB_DICT_ITERATE_FOR(&resources->textures, iter)
	{
		IgnisTexture2D* tex = (IgnisTexture2D*)clib_dict_iter_get_value(iter);
		ignisDeleteTexture2D(tex);
		free(tex);
		clib_dict_iter_remove(&resources->textures, iter);
	}

	CLIB_DICT_ITERATE_FOR(&resources->fonts, iter)
	{
		IgnisFont* font = (IgnisFont*)clib_dict_iter_get_value(iter);
		ignisDeleteFont(font);
		free(font);
		clib_dict_iter_remove(&resources->fonts, iter);
	}
}

IgnisTexture2D* ResourceManagerAddTexture2D(ResourceManager* resources, const char* name, const char* path, int rows, int columns)
{
	IgnisTexture2D* texture = (IgnisTexture2D*)malloc(sizeof(IgnisTexture2D));

	if (ignisCreateTexture2D(texture, path, rows, columns, 1, NULL))
	{
		if (clib_dict_insert(&resources->textures, name, texture) == texture)
			return texture;

		DEBUG_ERROR("[Resources] Failed to add texture: %s (%s)\n", name, path);
		ignisDeleteTexture2D(texture);
	}

	free(texture);
	return NULL;
}

IgnisFont* ResourceManagerAddFont(ResourceManager* manager, const char* name, const char* path, float size)
{
	IgnisFont* font = (IgnisFont*)malloc(sizeof(IgnisFont));

	if (ignisCreateFont(font, path, size))
	{
		if (clib_dict_insert(&manager->fonts, name, font) == font)
			return font;

		DEBUG_ERROR("[Resources] Failed to add font: %s (%s)\n", name, path);
		ignisDeleteFont(font);
	}
	free(font);
	return NULL;
}

IgnisTexture2D* ResourceManagerGetTexture2D(ResourceManager* manager, const char* name)
{
	IgnisTexture2D* tex = clib_dict_find(&manager->textures, name);

	if (!tex) DEBUG_WARN("[Resources] Could not find texture: %s\n", name);

	return tex;
}

IgnisFont* ResourceManagerGetFont(ResourceManager* manager, const char* name)
{
	IgnisFont* font = clib_dict_find(&manager->fonts, name);

	if (!font) DEBUG_WARN("[Resources] Could not find font: %s\n", name);

	return font;
}

const char* ResourceManagerGetTexture2DName(ResourceManager* resources, IgnisTexture2D* texture)
{
	CLIB_DICT_ITERATE_FOR(&resources->textures, iter)
	{
		if (texture == clib_dict_iter_get_value(iter))
			return clib_dict_iter_get_key(iter);
	}

	return NULL;
}

const char* ResourceManagerGetFontName(ResourceManager* resources, IgnisFont* font)
{
	CLIB_DICT_ITERATE_FOR(&resources->fonts, iter)
	{
		if (font == clib_dict_iter_get_value(iter))
			return clib_dict_iter_get_key(iter);
	}

	return NULL;
}
