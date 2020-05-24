#include "ResourceManager.h"

#define TB_JSON_IMPLEMENTATION
#include "json/tb_json.h"
#include "clib/clib.h"

#include "Application/Debugger.h"
#include "Application/defines.h"

CLIB_DICT_DEFINE_FUNCS(tex, IgnisTexture2D)
CLIB_DICT_DEFINE_FUNCS(font, IgnisFont)

int ResourceManagerInit(ResourceManager* resources, const char* path)
{
	clib_dict_init(&resources->textures, 0);
	clib_dict_init(&resources->fonts, 0);

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

		int rows = MAX(tb_json_int((char*)texture.value, "{'atlas'[0", NULL), 1);
		int columns = MAX(tb_json_int((char*)texture.value, "{'atlas'[1", NULL), 1);

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

		float font_size = tb_json_float((char*)font.value, "{'size'", NULL);

		ResourceManagerAddFont(resources, font_name, font_path, font_size);
	}

	free(json);

	return 1;
}

void ResourceManagerDestroy(ResourceManager* manager)
{
	for (clib_dict_iter* iter = clib_dict_iterator(&manager->textures); iter; iter = clib_dict_iter_next(&manager->textures, iter))
		ignisDeleteTexture2D(tex_dict_iter_get_value(iter));

	clib_dict_destroy(&manager->textures);

	for (clib_dict_iter* iter = clib_dict_iterator(&manager->fonts); iter; iter = clib_dict_iter_next(&manager->fonts, iter))
		ignisDeleteFont(font_dict_iter_get_value(iter));

	clib_dict_destroy(&manager->fonts);
}

IgnisTexture2D* ResourceManagerAddTexture2D(ResourceManager* manager, const char* name, const char* path, int rows, int columns)
{
	if (strlen(name) > APPLICATION_STR_LEN)
	{
		DEBUG_ERROR("[Resources] Texture name (%s) too long. Max. name length is %d\n", name, APPLICATION_STR_LEN);
		return NULL;
	}

	IgnisTexture2D* texture = (IgnisTexture2D*)malloc(sizeof(IgnisTexture2D));

	if (ignisCreateTexture2D(texture, path, rows, columns, 1, NULL))
	{
		if (tex_dict_insert(&manager->textures, name, texture) == texture)
			return texture;

		DEBUG_ERROR("[Resources] Failed to add texture: %s (%s)\n", name, path);
		ignisDeleteTexture2D(texture);
	}
	free(texture);
	return NULL;
}

IgnisFont* ResourceManagerAddFont(ResourceManager* manager, const char* name, const char* path, float size)
{
	if (strlen(name) > APPLICATION_STR_LEN)
	{
		DEBUG_ERROR("[Resources] Font name (%s) too long. Max. name length is %d\n", name, APPLICATION_STR_LEN);
		return NULL;
	}

	IgnisFont* font = (IgnisFont*)malloc(sizeof(IgnisFont));

	if (ignisCreateFont(font, path, size))
	{
		if (font_dict_insert(&manager->fonts, name, font) == font)
			return font;

		DEBUG_ERROR("[Resources] Failed to add font: %s (%s)\n", name, path);
		ignisDeleteFont(font);
	}
	free(font);
	return NULL;
}

IgnisTexture2D* ResourceManagerGetTexture2D(ResourceManager* manager, const char* name)
{
	IgnisTexture2D* tex = tex_dict_get(&manager->textures, name);

	if (tex) return tex;

	DEBUG_WARN("[Resources] Could not find texture: %s\n", name);
	return NULL;
}

IgnisFont* ResourceManagerGetFont(ResourceManager* manager, const char* name)
{
	IgnisFont* font = font_dict_get(&manager->fonts, name);

	if (font) return font;

	DEBUG_WARN("[Resources] Could not find font: %s\n", name);
	return NULL;
}

const char* ResourceManagerGetTexture2DName(ResourceManager* resources, IgnisTexture2D* texture)
{
	for (clib_dict_iter* iter = clib_dict_iterator(&resources->textures); iter; iter = clib_dict_iter_next(&resources->textures, iter))
	{
		if (texture == tex_dict_iter_get_value(iter))
			return tex_dict_iter_get_key(iter);
	}

	return NULL;
}
