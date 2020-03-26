#include "ResourceManager.h"

#include "json/tb_json.h"
#include "clib/clib.h"

struct _texkvp
{
	char key[RESOURCE_MANAGER_KEYLEN];
	IgnisTexture* value;
};

struct _fontkvp
{
	char key[RESOURCE_MANAGER_KEYLEN];
	IgnisFont* value;
};

/* Declare type-specific type_hashmap_* functions with this handy macro */
HASHMAP_DEFINE_FUNCS(tex, char, struct _texkvp)
HASHMAP_DEFINE_FUNCS(font, char, struct _fontkvp)

struct _texkvp* _tex_kvp(const char* key, IgnisTexture* value)
{
	if (strlen(key) > RESOURCE_MANAGER_KEYLEN)
		return NULL;

	struct _texkvp* kvp = (struct _texkvp*)malloc(sizeof(struct _texkvp));

	if (kvp)
	{
		strcpy(kvp->key, key);
		kvp->value = value;
	}

	return kvp;
}

struct _fontkvp* _font_kvp(const char* key, IgnisFont* value)
{
	if (strlen(key) > RESOURCE_MANAGER_KEYLEN)
		return NULL;

	struct _fontkvp* kvp = (struct _fontkvp*)malloc(sizeof(struct _fontkvp));

	if (kvp)
	{
		strcpy(kvp->key, key);
		kvp->value = value;
	}

	return kvp;
}

int ResourceManagerInit(ResourceManager* manager, const char* json_path)
{
	hashmap_init(&manager->textures, hashmap_hash_string, hashmap_compare_string, 0);
	hashmap_init(&manager->fonts, hashmap_hash_string, hashmap_compare_string, 0);

	char* json = ignisReadFile(json_path, NULL);

	if (!json)
	{
		printf("Index not found (%s)", json_path);
		return 0;
	}

	// Textures
	tb_json_element textures;
	tb_json_read(json, &textures, "{'textures'");

	for (int i = 0; i < textures.elements; i++)
	{
		char name[RESOURCE_MANAGER_KEYLEN];
		tb_json_string((char*)textures.value, "{*", name, RESOURCE_MANAGER_KEYLEN, &i);

		tb_json_element texture;
		tb_json_read_format((char*)textures.value, &texture, "{'%s'", name);

		char path[RESOURCE_MANAGER_PATHLEN];
		tb_json_string((char*)texture.value, "{'path'", path, RESOURCE_MANAGER_PATHLEN, NULL);

		int rows = MAX(tb_json_int((char*)texture.value, "{'atlas'[0", NULL), 1);
		int columns = MAX(tb_json_int((char*)texture.value, "{'atlas'[1", NULL), 1);

		ResourceManagerAddTexture(manager, name, path, rows, columns);
	}

	// Fonts
	tb_json_element fonts;
	tb_json_read(json, &fonts, "{'fonts'");

	for (int i = 0; i < fonts.elements; i++)
	{
		char name[RESOURCE_MANAGER_KEYLEN];
		tb_json_string((char*)fonts.value, "{*", name, RESOURCE_MANAGER_KEYLEN, &i);

		tb_json_element font;
		tb_json_read_format((char*)fonts.value, &font, "{'%s'", name);

		char path[RESOURCE_MANAGER_PATHLEN];
		tb_json_string((char*)font.value, "{'path'", path, RESOURCE_MANAGER_PATHLEN, NULL);

		float size = tb_json_float((char*)font.value, "{'size'", NULL);

		ResourceManagerAddFont(manager, name, path, size);
	}

	free(json);

	return 1;
}

void ResourceManagerDestroy(ResourceManager* manager)
{
	for (struct hashmap_iter* iter = hashmap_iter(&manager->textures); iter; iter = hashmap_iter_next(&manager->textures, iter))
		ignisDestroyTexture(tex_hashmap_iter_get_value(iter)->value);

	hashmap_destroy(&manager->textures);

	for (struct hashmap_iter* iter = hashmap_iter(&manager->fonts); iter; iter = hashmap_iter_next(&manager->fonts, iter))
		ignisDeleteFont(font_hashmap_iter_get_value(iter)->value);

	hashmap_destroy(&manager->fonts);
}

IgnisTexture* ResourceManagerAddTexture(ResourceManager* manager, const char* name, const char* path, int rows, int columns)
{
	IgnisTexture* texture = (IgnisTexture*)malloc(sizeof(IgnisTexture));

	if (ignisCreateTexture(texture, path, rows, columns, 1, NULL))
	{
		struct _texkvp* kvp = _tex_kvp(name, texture);

		if (kvp && tex_hashmap_put(&manager->textures, kvp->key, kvp) == kvp)
			return texture;

		ignisDestroyTexture(texture);
		free(kvp);
	}
	free(texture);
	return NULL;
}

IgnisTexture* ResourceManagerGetTexture(ResourceManager* manager, const char* name)
{
	struct _texkvp* kvp = tex_hashmap_get(&manager->textures, name);

	if (kvp) return kvp->value;

	return NULL;
}

IgnisFont* ResourceManagerAddFont(ResourceManager* manager, const char* name, const char* path, float size)
{
	IgnisFont* font = (IgnisFont*)malloc(sizeof(IgnisFont));

	if (ignisLoadFont(font, path, size))
	{
		struct _fontkvp* kvp = _font_kvp(name, font);

		if (kvp && font_hashmap_put(&manager->fonts, kvp->key, kvp) == kvp)
			return font;
			
		ignisDeleteFont(font);
		free(kvp);
	}
	free(font);
	return NULL;
}

IgnisFont* ResourceManagerGetFont(ResourceManager* manager, const char* name)
{
	struct _fontkvp* kvp = font_hashmap_get(&manager->fonts, name);

	if (kvp) return kvp->value;

	return NULL;
}
